package com.ffvideo;

import android.app.Activity;
import android.os.Bundle;
import android.content.Context;
import android.util.AttributeSet;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class ffvideoActivity extends Activity {
	private FFMpegPlayerAndroid 	mPlayer;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		mPlayer = new FFMpegPlayerAndroid(this);

		setContentView(mPlayer);
	}
}

class FFMpegPlayerAndroid extends SurfaceView {
	private int 							mSurfaceWidth;
	private int 							mSurfaceHeight;
	private Thread							mRenderThread;
	private SurfaceHolder					mSurfaceHolder;
	private boolean							mPlaying;
	
	public static final String[] LIBS = new String[] {
		"ffplay", // used for access to android native AudioTrack class 
		"ffvideo"	// ffmpeg libs compiled to jni lib
	};
	
	private static boolean sLoaded = false;

    private static boolean loadLibs() {
    	if(sLoaded) {
    		return true;
    	}
    	boolean err = false;
    	for(int i=0;i<LIBS.length;i++) {
    		try {
    			System.loadLibrary(LIBS[i]);
    		} catch(UnsatisfiedLinkError e) {
    			// fatal error, we can't load some our libs
    			err = true;
    		}
		}
    	if(!err) {
    		sLoaded = true;
    	}
    	return sLoaded;
    }
    
	public FFMpegPlayerAndroid(Context context) {
        super(context);
        initVideoView(context);
    }
    
    public FFMpegPlayerAndroid(Context context, AttributeSet attrs) {
        this(context, attrs, 0);
        initVideoView(context);
    }
    
    public FFMpegPlayerAndroid(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
        initVideoView(context);
    }
    
    private void initVideoView(Context context) {
    	getHolder().addCallback(mSHCallback);
    	
    	loadLibs();
    }

	SurfaceHolder.Callback mSHCallback = new SurfaceHolder.Callback() {
        public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
            mSurfaceWidth = w;
            mSurfaceHeight = h;

    		mPlaying = true;		

    		if(mRenderThread == null) { // we hasn't run player thread so we are launching	
    			
    			mRenderThread = new Thread() {	
    				
    				public void run() {
    					String fname = "/sdcard/400x512_565.bmp";
    					nativePlay(fname, 400, 512);    						
    				}
    			};
    			mRenderThread.start();
    		}
        }

        public void surfaceCreated(SurfaceHolder holder) {
            mSurfaceHolder = holder;
            nativeSetSurface(mSurfaceHolder.getSurface());
        }

        public void surfaceDestroyed(SurfaceHolder holder) {
//			release();
            mSurfaceHolder = null;
        }
    };

	private native void   nativePlay(String fname, int width, int height);	

	private native void   nativeSetSurface(Surface surface);
}
