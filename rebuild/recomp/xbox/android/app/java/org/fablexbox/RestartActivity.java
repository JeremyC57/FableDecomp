package org.fablexbox;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;

import java.io.File;

/**
 * Restarts the game after it rebooted itself (loading a save from the pause menu reboots the
 * Xbox title with the save to load): waits for the old game process to exit, then starts a new
 * one with the same settings. Runs in the app's main process.
 */
public class RestartActivity extends Activity {
    private final Handler handler = new Handler(Looper.getMainLooper());
    private int waited;

    @Override
    protected void onCreate(Bundle saved) {
        super.onCreate(saved);
        handler.postDelayed(this::check, 200);
    }

    private void check() {
        int pid = getIntent().getIntExtra("pid", 0);
        if (pid != 0 && new File("/proc/" + pid).exists() && waited++ < 50) {
            handler.postDelayed(this::check, 100);
            return;
        }
        Intent game = getIntent().getParcelableExtra("game");
        if (game != null) {
            game.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TASK);
            startActivity(game);
        }
        finish();
    }
}
