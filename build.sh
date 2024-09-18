# You really think need this, isn't?
# export ANDROID_NDK=<path-to-ndk>

echo "[BUILD] Starting NDK..."
exec $ANDROID_NDK/ndk-build NDK_PROJECT_PATH=./ APP_BUILD_SCRIPT=./Android.mk NDK_APPLICATION_MK=./Application.mk NDK_DEBUG=0
echo "[BUILD] Done!"