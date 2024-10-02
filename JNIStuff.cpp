//
// JNIStuff.cpp
//
// Simple JNI Utils for Android
//
// Created by MrVenumx on 02/10/2024

#include "./JNIStuff.h"

JNIStuff::JNIStuff() {}

JNIStuff::~JNIStuff()
{
    JNIStuff::jvm->DetachCurrentThread();
    delete this->env;
    delete this->jvm;
}

void JNIStuff::setJavaVM(const JavaVM *jvm)
{
    this->jvm = (JavaVM *)jvm;

    if (this->jvm == nullptr)
        return;

    if (!(jint)this->jvm->AttachCurrentThread(&(this->env), nullptr) == JNI_OK)
    {
        JNISTUFF_LOGE("Failed Attach, cannot attach to current thread");
        return;
    }
}

jobject JNIStuff::getGlobalContext(JNIStuff *instance)
{
    if (instance == nullptr)
    {
        JNISTUFF_LOGE("Instance is Null, Operation cannot proceed.");
        return nullptr;
    }

    jclass activityThread = instance->env->FindClass("android/app/ActivityThread");
    jmethodID currentActivityThread = instance->env->GetStaticMethodID(activityThread, "currentActivityThread", "()Landroid/app/ActivityThread;");
    jobject at = instance->env->CallStaticObjectMethod(activityThread, currentActivityThread);
    jmethodID getApplication = instance->env->GetMethodID(activityThread, "getApplication", "()Landroid/app/Application;");
    jobject context = instance->env->CallObjectMethod(at, getApplication);
    return context;
}

jstring JNIStuff::getAppName(JNIStuff *instance, jobject context)
{
    if (instance == nullptr)
    {
        JNISTUFF_LOGE("Instance is Null, Operation cannot proceed.");
        return nullptr;
    }

    jclass contextClass = instance->env->GetObjectClass(context);
    jmethodID getPackageManager = instance->env->GetMethodID(contextClass, "getPackageManager", "()Landroid/content/pm/PackageManager;");
    jobject packageManager = instance->env->CallObjectMethod(context, getPackageManager);

    jmethodID getPackageName = instance->env->GetMethodID(contextClass, "getPackageName", "()Ljava/lang/String;");
    jstring packageName = (jstring)instance->env->CallObjectMethod(context, getPackageName);

    jclass packageManagerClass = instance->env->GetObjectClass(packageManager);
    jmethodID getApplicationInfo = instance->env->GetMethodID(packageManagerClass, "getApplicationInfo", "(Ljava/lang/String;I)Landroid/content/pm/ApplicationInfo;");
    jobject applicationInfo = instance->env->CallObjectMethod(packageManager, getApplicationInfo, packageName, 0);

    jclass applicationInfoClass = instance->env->GetObjectClass(applicationInfo);
    jmethodID loadLabel = instance->env->GetMethodID(applicationInfoClass, "loadLabel", "(Landroid/content/pm/PackageManager;)Ljava/lang/CharSequence;");
    return (jstring)instance->env->CallObjectMethod(applicationInfo, loadLabel, packageManager);
}

jstring JNIStuff::getPackageName(JNIStuff *instance, jobject context)
{
    jclass jActivity_class = instance->env->GetObjectClass(context);

    jmethodID jMethod_id_pn = instance->env->GetMethodID(jActivity_class, "getPackageName", "()Ljava/lang/String;");
    return (jstring)instance->env->CallObjectMethod(context, jMethod_id_pn);
}

jobject JNIStuff::getAssets(JNIStuff *instance, jobject context)
{
    jclass JActivity_class = instance->env->GetObjectClass(context);

    jmethodID jMethod_getAssets = instance->env->GetMethodID(JActivity_class, "getAssets", "()Landroid/content/res/AssetManager;");
    return (jobject)instance->env->CallObjectMethod(context, jMethod_getAssets);
}

jstring JNIStuff::getAndroidPermission(JNIStuff *instance, const char *szPermissionName)
{
    if (instance == nullptr)
    {
        JNISTUFF_LOGE("Instance is Null, Operation cannot proceed.");
        return nullptr;
    }

    jclass ClassManifestPermission = instance->env->FindClass("android/Manifest$permission");
    jfieldID lid_PERM = instance->env->GetStaticFieldID(ClassManifestPermission, szPermissionName, "Ljava/lang/String;");
    return (jstring)instance->env->GetStaticObjectField(ClassManifestPermission, lid_PERM);
}

bool JNIStuff::hasPermissionGranted(JNIStuff *instance, jobject context, const char *szPermissionName)
{
    jclass ClassPackageManager = instance->env->FindClass("android/content/pm/PackageManager");
    jclass ClassManifestPermission = instance->env->FindClass("android/Manifest$permission");

    jfieldID lid_PERM = instance->env->GetStaticFieldID(ClassManifestPermission, szPermissionName, "Ljava/lang/String;");
    jstring ls_PERM = (jstring)instance->env->GetStaticObjectField(ClassManifestPermission, lid_PERM);

    jfieldID lid_PERMISSION_GRANTED = instance->env->GetStaticFieldID(ClassPackageManager, "PERMISSION_GRANTED", "I");
    jint PERMISSION_GRANTED = jint(-1);
    PERMISSION_GRANTED = instance->env->GetStaticIntField(ClassPackageManager, lid_PERMISSION_GRANTED);
    jint int_result = instance->env->CallIntMethod(context, instance->env->GetMethodID(instance->env->FindClass("android/content/Context"), "checkSelfPermission", "(Ljava/lang/String;)I"), ls_PERM);
    return (int_result == PERMISSION_GRANTED);
}

void JNIStuff::showSoftKeyboard(JNIStuff *instance, jobject context)
{
    if (instance == nullptr)
    {
        JNISTUFF_LOGE("Instance is Null, Operation cannot proceed.");
        return;
    }

    jclass contextClass = instance->env->GetObjectClass(context);
    jmethodID getSystemService = instance->env->GetMethodID(contextClass, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
    jobject inputMethodManager = instance->env->CallObjectMethod(context, getSystemService, instance->env->NewStringUTF("input_method"));

    jclass inputMethodManagerClass = instance->env->GetObjectClass(inputMethodManager);
    jmethodID showSoftInput = instance->env->GetMethodID(inputMethodManagerClass, "toggleSoftInput", "(II)V");
    instance->env->CallVoidMethod(inputMethodManager, showSoftInput, 0, 0);
}

void JNIStuff::showToast(JNIStuff *instance, jobject context, const char *text, int longer)
{
    if (instance == nullptr)
    {
        JNISTUFF_LOGE("Instance is Null, Operation cannot proceed.");
        return;
    }

    jclass toast = instance->env->FindClass("android/widget/Toast");
    jmethodID methodMakeText = instance->env->GetStaticMethodID(toast, "makeText", "(Landroid/content/Context;Ljava/lang/CharSequence;I)Landroid/widget/Toast;");
    jobject toastobj = instance->env->CallStaticObjectMethod(toast, methodMakeText, context, instance->env->NewStringUTF(text), longer);
    jmethodID methodShow = instance->env->GetMethodID(toast, "show", "()V");
    instance->env->CallVoidMethod(toastobj, methodShow);
}

const char *JNIStuff::jstring2cstring(JNIStuff *instance, jstring string)
{
    if (instance == nullptr)
    {
        JNISTUFF_LOGE("Instance is Null, Operation cannot proceed.");
        return nullptr;
    }

    return (char *)instance->env->GetStringUTFChars(string, 0);
}