# Patchx

Simple internal Modding library for Android.

# What's Patchx?

Patchx it's a simple Modding Template for Patching and Hook using **DobbyHook**, **GloosHook**, and **KittyMemory** for Android.

# How To Implement?

You need this required Tools for insert `libPatchx.so` into Apk

-   ApkTool
-   Any TextEditor

And for **Android** Users you can use **MT Manager**.

For **ApkTool** you can follow this intruction:

1. Decompile Target APK using `java -jar apktool.jar d target.apk`
2. Copy `Patchx/libs` to `Target-src/`, if already exist, you can insert directly into `Patchx/libs/*` depend on your SoC Architecture.
3. Find MainActivity on `Target-src/smali`, make sure you not use `--no-src`
4. Open with your TextEditor like **Notepad++** or **VsCode**
5. Put this code inside `OnCreate` Method. Please make sure `registers` above _1_.

```
const-string v0, "Patchx" # Patchx library name

invoke-static {v0}, Ljava/lang/System;->loadLibrary(Ljava/lang/String;)V
```

6. Compile again Target APK using `java -jar apktool.jar b <path-to-src>`
7. Then install Target APK you already Patch. I recommended using **ADB** for checking Logcat using `adb logcat -s Patchx` to check.

For **MT Manager** you can follow this intruction:

1. Open MT Manager
2. Extract APK or Find your `target.apk`
3. View Source by click `target.apk` then click View
4. Edit Dex by click `classes.dex` then select **Dex Editor plus**
5. Do same procces by searching MainActivity and put the code to `OnCreate` method.
6. Press Back Button and click **Save & Exit**
7. Then install Target APK you already Patch using **MT Manager**

NOTE: I recommended you for change or use '''android:extractNativeLibs="true"''' for prevent some issues.

By following this step, now you success to Patch you own APK.

# Know Issues

These are some of the issues i have found.

-   **Split APK** will not work because Library name will not found.

# Credits

Thanks to some of these Projects that have provided tools for this project

-   LGLTeam - Based on: https://github.com/LGLTeam/Android-Mod-Menu
-   MJx0 - Memory Patch: https://github.com/MJx0/KittyMemory
-   Jmpews - A multi-architecture Hook: https://github.com/jmpews/Dobby
-   XMDS - Advance Hook & Memory for ARM:https://github.com/XMDS/GlossHook
-   And, Thanks to person who gave me the idea for this project:)

# Disclaimer!

This Project is for **Educational** only, We are not responsible for anything that happens to it! I made this because bored:)
