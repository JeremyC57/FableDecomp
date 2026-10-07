package org.fablexbox;

import android.content.Context;
import android.os.Bundle;
import android.os.VibrationEffect;
import android.os.Vibrator;
import android.system.Os;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.ViewGroup;
import android.widget.FrameLayout;

import org.libsdl.app.SDLActivity;

import java.io.File;

/** Runs the game (libmain.so, via SDL) with the launcher's settings, plus the touch overlay. */
public class GameActivity extends SDLActivity {
    static native float[] nativeStats();
    static native void nativeSetOption(String name, boolean on);
    static native String nativeProfile(int seconds);

    private TouchControls touch;
    private QuickMenu menu;

    @Override
    protected void onCreate(Bundle saved) {
        File data = LauncherActivity.dataDir(this);
        data.mkdirs();
        String driver = getIntent().getStringExtra("driver");
        String lib = Drivers.library(this, driver);
        try {
            Os.setenv("FABLE_XBOX_DATA", data.getAbsolutePath(), true);
            Os.setenv("FABLE_NATIVE_LIB_DIR", getApplicationInfo().nativeLibraryDir, true);
            Os.setenv("FABLE_TMP_DIR", getCacheDir().getAbsolutePath(), true);
            if (lib != null) {
                Os.setenv("FABLE_VK_DRIVER_DIR", new File(Drivers.root(this), driver).getAbsolutePath() + "/", true);
                Os.setenv("FABLE_VK_DRIVER_LIB", lib, true);
            } else {
                Os.unsetenv("FABLE_VK_DRIVER_DIR");
                Os.unsetenv("FABLE_VK_DRIVER_LIB");
            }
        } catch (Exception e) {
            Log.e("FableXbox", "setenv failed", e);
        }
        super.onCreate(saved);
        if (mLayout != null) {
            touch = new TouchControls(this, getIntent().getBooleanExtra("touch", true));
            mLayout.addView(touch, new ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT));
            FrameLayout overlay = new FrameLayout(this);
            mLayout.addView(overlay, new ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT));
            menu = new QuickMenu(this, overlay, touch, new QuickMenu.Natives() {
                @Override public float[] stats() { return nativeStats(); }
                @Override public void setOption(String name, boolean on) { nativeSetOption(name, on); }
                @Override public String profile(int seconds) { return nativeProfile(seconds); }
            });
        }
    }

    /** A swipe from the left edge opens the quick menu (before the game and the controls see it). */
    @Override
    public boolean dispatchTouchEvent(MotionEvent e) {
        if (menu != null && menu.onTouch(e)) return true;
        return super.dispatchTouchEvent(e);
    }

    @Override
    public boolean dispatchKeyEvent(KeyEvent e) {
        if (menu != null && menu.isOpen() && e.getKeyCode() == KeyEvent.KEYCODE_BACK) {
            if (e.getAction() == KeyEvent.ACTION_UP) menu.close();
            return true;
        }
        return super.dispatchKeyEvent(e);
    }

    /**
     * Called from native code (input.cpp) with the game's XInput motor speeds (0..65535) when no
     * game controller is connected; renewed about every 500 ms while the motors run, 0/0 stops.
     */
    public void vibrate(int left, int right) {
        Vibrator v = (Vibrator) getSystemService(Context.VIBRATOR_SERVICE);
        if (v == null || !v.hasVibrator()) return;
        int speed = Math.max(left, right);
        if (speed == 0) {
            v.cancel();
            return;
        }
        int amplitude = v.hasAmplitudeControl() ? Math.max(1, Math.min(255, speed / 257)) : VibrationEffect.DEFAULT_AMPLITUDE;
        v.vibrate(VibrationEffect.createOneShot(700, amplitude));
    }

    @Override
    protected String[] getLibraries() {
        return new String[] {"SDL2", "main"};
    }

    @Override
    protected String[] getArguments() {
        String dir = getIntent().getStringExtra("gameDir");
        String config = getIntent().getStringExtra("config");
        File hdd = new File(LauncherActivity.dataDir(this), "hdd");
        return new String[] {"--game", dir != null ? dir : "", "--hdd", hdd.getAbsolutePath(), "--config", config != null ? config : ""};
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        // The native side cannot be restarted inside this process; the next launch gets a new one.
        android.os.Process.killProcess(android.os.Process.myPid());
    }
}
