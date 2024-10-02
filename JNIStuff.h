//
// JNIStuff.h
//
// Simple JNI Utils for Android
//
// Created by MrVenumx on 02/10/2024

#ifndef JNISTUFF_H_2
#define JNISTUFF_H_2

#include <jni.h>

#include <android/log.h>

#define JNISTUFF_TAG "JNIStuff"

#define JNISTUFF_LOGD(...) ((void)__android_log_print(3, JNISTUFF_TAG, __VA_ARGS__))
#define JNISTUFF_LOGE(...) ((void)__android_log_print(6, JNISTUFF_TAG, __VA_ARGS__))
#define JNISTUFF_LOGI(...) ((void)__android_log_print(4, JNISTUFF_TAG, __VA_ARGS__))
#define JNISTUFF_LOGW(...) ((void)__android_log_print(5, JNISTUFF_TAG, __VA_ARGS__))

class JNIStuff
{
private:
public:
    JavaVM *jvm;
    JNIEnv *env;

    JNIStuff();
    ~JNIStuff();

    void setJavaVM(const JavaVM *jvm);

    static jobject getGlobalContext(JNIStuff *instance);
    static jstring getAppName(JNIStuff *instance, jobject context);
    static jstring getPackageName(JNIStuff *instance, jobject context);
    static jobject getAssets(JNIStuff *instance, jobject context);
    static jstring getAndroidPermission(JNIStuff *instance, const char *szPermissionName);
    static bool hasPermissionGranted(JNIStuff *instance, jobject context, const char *szPermissionName);
    static void showSoftKeyboard(JNIStuff *instance, jobject context);
    static void showToast(JNIStuff *instance, jobject context, const char *text, int longer);
    static const char *jstring2cstring(JNIStuff *instance, jstring string);
};

#endif