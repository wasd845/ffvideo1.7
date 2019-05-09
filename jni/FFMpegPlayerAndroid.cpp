#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "drivers_map.h"

static int jniRegisterNativeMethods(JNIEnv* env, const char* className, const JNINativeMethod* gMethods, int numMethods)
{
    jclass clazz;

    clazz = env->FindClass(className);
    if (clazz == NULL)
        return -1;

    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0)
        return -1;

    return 0;
}

static void FFMpegPlayerAndroid_play(JNIEnv *env, jobject obj, jstring fname, jint width, jint height) 
{	
	jboolean isCopy;
    const char * szfname = env->GetStringUTFChars(fname, &isCopy);
	FILE *fp=fopen(szfname, "rb"); // "/sdcard/512x400_565.bmp", "rb");
	char *buf=(char*)malloc(width*height*2+70);
	fread(buf, 1, width*height*2+70, fp);
	fclose(fp);
    env->ReleaseStringUTFChars(fname, szfname);
    			
	while(1)
	{
		void * pbits;
				
		VideoDriver_getPixels(width, height, &pbits);
		
		memcpy(pbits, buf+70, width*height*2);
		
		VideoDriver_updateSurface();
		
		usleep(1000);
	}
}

static void FFMpegPlayerAndroid_setSurface(JNIEnv *env, jobject obj, jobject jsurface) 
{
	VideoDriver_register(env, jsurface);
}

static JNINativeMethod methods[] = {
	{ "nativePlay", "(Ljava/lang/String;II)V", (void*) FFMpegPlayerAndroid_play },
	{ "nativeSetSurface", "(Landroid/view/Surface;)V", (void*) FFMpegPlayerAndroid_setSurface },
};
	
int register_FFMpegPlayerAndroid(JNIEnv *env) 
{
	return jniRegisterNativeMethods(env, "com/ffvideo/FFMpegPlayerAndroid", methods, sizeof(methods) / sizeof(methods[0]));
}
