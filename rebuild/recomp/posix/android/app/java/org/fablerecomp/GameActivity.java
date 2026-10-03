package org.fablerecomp;

import android.os.Bundle;
import android.system.Os;
import android.util.Log;
import android.view.ViewGroup;

import org.libsdl.app.SDLActivity;

import java.io.File;

/** Runs the game (libmain.so, via SDL) with the launcher's settings, plus the touch overlay. */
public class GameActivity extends SDLActivity {
    private TouchControls touch;

    @Override
    protected void onCreate(Bundle saved) {
        File data = LauncherActivity.dataDir(this);
        data.mkdirs();
        String driver = getIntent().getStringExtra("driver");
        String lib = Drivers.library(this, driver);
        try {
            Os.setenv("FABLE_RECOMP_DATA", data.getAbsolutePath(), true);
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
            Log.e("FableRecomp", "setenv failed", e);
        }
        super.onCreate(saved);
        if (mLayout != null) {
            touch = new TouchControls(this, getIntent().getBooleanExtra("touch", true));
            mLayout.addView(touch, new ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT));
        }
    }

    @Override
    protected String[] getLibraries() {
        return new String[] {"SDL2", "main"};
    }

    @Override
    protected String[] getArguments() {
        String dir = getIntent().getStringExtra("gameDir");
        float scale = getIntent().getFloatExtra("uiScale", 1f);
        return new String[] {"--game", dir != null ? dir : "", "--ui-scale=" + Math.round(scale * 100f)};
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        // The native side cannot be restarted inside this process; the next launch gets a new one.
        android.os.Process.killProcess(android.os.Process.myPid());
    }
}
