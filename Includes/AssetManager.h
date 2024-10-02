// Simple header for access '/assets' on Android.
//
// Created by MrVenumX on 02/10/2024

#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>
#include <JNIStuff.h>

#define ASSETMANAGER "AssetManager"

#define ASSETMANAGER_LOGE(...) ((void)__android_log_print(6, ASSETMANAGER, __VA_ARGS__))
#define ASSETMANAGER_LOGI(...) ((void)__android_log_print(4, ASSETMANAGER, __VA_ARGS__))
#define ASSETMANAGER_LOGW(...) ((void)__android_log_print(5, ASSETMANAGER, __VA_ARGS__))

class AssetManager
{
private:
    /* Nothing here*/
public:
    AAssetManager *assetManager;
    JNIStuff *JStuff;

    AssetManager(JNIStuff *instance);
    ~AssetManager();
    /**
     * Reading content from Android Assets files
     *
     * Returns nullptr on failure
     */
    const char *readContent(const char *filename);

    /**
     * Get content length from Android Assets files
     *
     * Return 0 or EOF on failure
     */
    off_t getContentLength(const char *filename);
};

AssetManager::AssetManager(JNIStuff *instance)
{

    if (instance->jvm == nullptr)
    {
        ASSETMANAGER_LOGE("JVM is Null, Operation cannot proceed");
        return;
    }

    if (instance->env == nullptr)
    {
        ASSETMANAGER_LOGE("JVM is Null, Operation cannot proceed");
        return;
    }

    jobject context = JNIStuff::getGlobalContext(instance);
    jobject jAssetManager = JNIStuff::getAssets(instance, context);

    assetManager = AAssetManager_fromJava(instance->env, jAssetManager);

    if (assetManager == nullptr)
    {
        ASSETMANAGER_LOGE("AssetManager initialize end with error: 'assetManager' is Null");
        return;
    }
}

AssetManager::~AssetManager()
{
    /* Nothing */
}

const char *AssetManager::readContent(const char *filename)
{
    off_t assetSize;
    char *buffers;
    AAsset *assetf;

    assetf = AAssetManager_open(this->assetManager, filename, AASSET_MODE_STREAMING);
    assetSize = AAsset_getLength(assetf);

    if (assetf == nullptr)
    {
        ASSETMANAGER_LOGE("'assetf' is null, Operation cannot proceed");
        return nullptr;
    }

    if (assetSize < 0)
    {
        ASSETMANAGER_LOGE("'assetSize' is 0, Operation cannot proceed");
        return nullptr;
    }

    buffers = new char[assetSize + 1];
    buffers[assetSize] = '\0';

    AAsset_read(assetf, buffers, assetSize);
    AAsset_close(assetf);
    return buffers;
}

off_t AssetManager::getContentLength(const char *filename)
{
    AAsset *assetf;

    assetf = AAssetManager_open(this->assetManager, filename, AASSET_MODE_STREAMING);
    if (assetf == nullptr)
    {
        ASSETMANAGER_LOGE("'AAssetManager_open' is Null, Operation cannot proceed");
        return -1;
    }

    return AAsset_getLength(assetf);
}

#endif