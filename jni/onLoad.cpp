#define TAG "ffmpeg_onLoad"

#include <stdlib.h>
#include <android/log.h>
#include <jni.h>

extern int register_FFMpegPlayerAndroid(JNIEnv *env);

static JavaVM *sVm;

// Throw an exception with the specified class and an optional message.
int jniThrowException(JNIEnv* env, const char* className, const char* msg) 
{
    jclass exceptionClass = env->FindClass(className);
    if (exceptionClass == NULL) 
        return -1;

    if (env->ThrowNew(exceptionClass, msg) != JNI_OK) {
    }

    return 0;
}

JNIEnv* getJNIEnv() 
{
    JNIEnv* env = NULL;
    if (sVm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) 
        return NULL;

    return env;
}

jint JNI_OnLoad(JavaVM* vm, void* reserved) 
{
    JNIEnv* env = NULL;
    jint result = JNI_ERR;
	sVm = vm;

    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) 
        return result;

    if(register_FFMpegPlayerAndroid(env) != JNI_OK)
    	goto end;

    result = JNI_VERSION_1_4;

end:
    return result;
}
