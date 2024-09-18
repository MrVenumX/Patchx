Write-Output "[BUILD] Starting NDK..."
& ndk-build NDK_PROJECT_PATH=$PSScriptRoot APP_BUILD_SCRIPT=$PSScriptRoot/Android.mk NDK_APPLICATION_MK=$PSScriptRoot/Application.mk NDK_DEBUG=0 -j12
Write-Output "[BUILD] Done!"

Exit $LASTEXITCODE