package org.fablexbox;

import android.app.Activity;
import android.content.SharedPreferences;
import android.graphics.Color;
import android.graphics.Typeface;
import android.os.Handler;
import android.os.Looper;
import android.view.Gravity;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.ScrollView;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;

/**
 * In-game quick menu: swipe right from the left screen edge to open it. It holds an FPS counter
 * and the options that apply while the game runs (touch controls, vibration, HUD position at
 * 16:9, text sharpening), a performance profile recorder and Quit. Choices are kept in the
 * launcher's preferences, so they also apply to the next start.
 */
public final class QuickMenu {
    interface Natives {
        float[] stats();
        void setOption(String name, boolean on);
        String profile(int seconds);
    }

    private final Activity activity;
    private final FrameLayout root;
    private final Natives natives;
    private final SharedPreferences prefs;
    private final TouchControls touch;
    private final Handler handler = new Handler(Looper.getMainLooper());
    private final float dp;
    private final View scrim;
    private final ScrollView panel;
    private final TextView fps;
    private boolean open, tracking;
    private float downX, downY;

    QuickMenu(Activity a, FrameLayout root, TouchControls touch, Natives natives) {
        this.activity = a;
        this.root = root;
        this.touch = touch;
        this.natives = natives;
        prefs = a.getSharedPreferences(LauncherActivity.PREFS, Activity.MODE_PRIVATE);
        dp = a.getResources().getDisplayMetrics().density;

        fps = new TextView(a);
        fps.setTextColor(Color.WHITE);
        fps.setTextSize(13);
        fps.setTypeface(Typeface.MONOSPACE, Typeface.BOLD);
        fps.setShadowLayer(3 * dp, 0, 0, Color.BLACK);
        fps.setPadding(px(8), px(4), px(8), px(4));
        FrameLayout.LayoutParams fl = new FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT, FrameLayout.LayoutParams.WRAP_CONTENT, Gravity.TOP | Gravity.CENTER_HORIZONTAL);
        root.addView(fps, fl);
        fps.setVisibility(prefs.getBoolean("fpsCounter", false) ? View.VISIBLE : View.GONE);

        scrim = new View(a);
        scrim.setBackgroundColor(Color.argb(90, 0, 0, 0));
        scrim.setVisibility(View.GONE);
        scrim.setOnClickListener(v -> close());
        root.addView(scrim, new FrameLayout.LayoutParams(FrameLayout.LayoutParams.MATCH_PARENT, FrameLayout.LayoutParams.MATCH_PARENT));

        panel = new ScrollView(a);
        panel.setBackgroundColor(Color.argb(235, 18, 20, 26));
        LinearLayout col = new LinearLayout(a);
        col.setOrientation(LinearLayout.VERTICAL);
        col.setPadding(px(16), px(16), px(16), px(16));
        panel.addView(col);

        TextView title = new TextView(a);
        title.setText("Quick menu");
        title.setTextColor(Color.WHITE);
        title.setTextSize(20);
        title.setTypeface(Typeface.DEFAULT_BOLD);
        title.setPadding(0, 0, 0, px(8));
        col.addView(title);

        col.addView(toggle("FPS counter", "fpsCounter", false, on -> {
            fps.setVisibility(on ? View.VISIBLE : View.GONE);
            if (on) poll();
        }));
        col.addView(toggle("Touch controls", "touch", true, on -> { if (touch != null) touch.setControlsShown(on); }));
        col.addView(toggle("Vibration", "vibration", true, on -> natives.setOption("vibration", on)));
        col.addView(toggle("HUD at the screen edges (16:9)", "hudCorners", true, on -> natives.setOption("hud_corners", on)));
        col.addView(toggle("Sharpen text", "textSharpen", true, on -> natives.setOption("text_sharpen", on)));

        Button profile = button("Record performance profile (15 s)");
        profile.setOnClickListener(v -> {
            String path = natives.profile(15);
            Toast.makeText(a, path != null ? "Recording 15 s of performance data to " + path + ". Keep playing." : "A recording is already running.",
                Toast.LENGTH_LONG).show();
            close();
        });
        col.addView(profile);
        Button resume = button("Resume");
        resume.setOnClickListener(v -> close());
        col.addView(resume);
        Button quit = button("Quit game");
        quit.setOnClickListener(v -> a.finish());
        col.addView(quit);

        panel.setVisibility(View.GONE);
        root.addView(panel, new FrameLayout.LayoutParams(px(300), FrameLayout.LayoutParams.MATCH_PARENT, Gravity.START));

        // The native side starts from the settings file; bring the live options in line with the menu.
        natives.setOption("hud_corners", prefs.getBoolean("hudCorners", true));
        natives.setOption("text_sharpen", prefs.getBoolean("textSharpen", true));
        if (fps.getVisibility() == View.VISIBLE) poll();
    }

    private interface OnToggle { void apply(boolean on); }

    private Switch toggle(String label, String key, boolean def, OnToggle f) {
        Switch s = new Switch(activity);
        s.setText(label);
        s.setTextColor(Color.WHITE);
        s.setTextSize(15);
        s.setPadding(0, px(10), 0, px(10));
        s.setChecked(prefs.getBoolean(key, def));
        s.setOnCheckedChangeListener((b, on) -> {
            prefs.edit().putBoolean(key, on).apply();
            f.apply(on);
        });
        return s;
    }

    private Button button(String label) {
        Button b = new Button(activity);
        b.setText(label);
        b.setAllCaps(false);
        LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
        lp.topMargin = px(8);
        b.setLayoutParams(lp);
        return b;
    }

    private void poll() {
        handler.removeCallbacksAndMessages(null);
        handler.post(new Runnable() {
            @Override public void run() {
                if (fps.getVisibility() != View.VISIBLE) return;
                float[] s = natives.stats();
                if (s != null && s.length >= 4)
                    fps.setText(String.format(java.util.Locale.US, "%.0f fps  ·  slowest %.0f ms  ·  %.0f passes  ·  GPU wait %.0f%%", s[0], s[1], s[2], s[3] * 100));
                else if (s != null && s.length >= 2) fps.setText(String.format(java.util.Locale.US, "%.0f fps  ·  slowest %.0f ms", s[0], s[1]));
                handler.postDelayed(this, 500);
            }
        });
    }

    void openMenu() {
        if (open) return;
        open = true;
        scrim.setVisibility(View.VISIBLE);
        panel.setVisibility(View.VISIBLE);
        panel.setTranslationX(-px(300));
        panel.animate().translationX(0).setDuration(160).start();
    }

    void close() {
        if (!open) return;
        open = false;
        scrim.setVisibility(View.GONE);
        panel.animate().translationX(-px(300)).setDuration(140).withEndAction(() -> { if (!open) panel.setVisibility(View.GONE); }).start();
    }

    /**
     * Called from Activity.dispatchTouchEvent before the game and the touch controls see the
     * event: a first finger that lands in the left-edge strip and moves right opens the menu.
     * Returns true when the event is consumed.
     */
    boolean onTouch(MotionEvent e) {
        if (open) return false;  // the menu views handle it
        switch (e.getActionMasked()) {
        case MotionEvent.ACTION_DOWN:
            tracking = e.getX() < px(20);
            downX = e.getX();
            downY = e.getY();
            return tracking;
        case MotionEvent.ACTION_MOVE:
            if (!tracking) return false;
            if (e.getX() - downX > px(48) && Math.abs(e.getY() - downY) < px(80)) {
                tracking = false;
                openMenu();
            }
            return true;
        case MotionEvent.ACTION_UP:
        case MotionEvent.ACTION_CANCEL:
            if (!tracking) return false;
            tracking = false;
            return true;
        default:
            return tracking;
        }
    }

    boolean isOpen() { return open; }

    private int px(float v) { return Math.round(v * dp); }
}
