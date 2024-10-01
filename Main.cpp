#include "Main.h"

#include "KittyMemory/MemoryPatch.h"
#include "Includes/GlossHook/Gloss.h"
#include "Includes/Dobby/dobby.h"
#include "Includes/Logger.h"
#include "Includes/Utils.h"
#include "Includes/Macros.h"
#include "Il2cpp/Il2cpp.hpp"

// DECL_HOOK(bool, MakeMeReturnTrue) {
//     return true;
// }

void *hack_thread(void *)
{
    LOGI("Patchx is loaded now >:)");
    do
    {
        libBase = (uintptr_t)GlossGetLibBias(libName);
        sleep(1);
        if (Utils::isUnityGame())
            Il2cppWrapper::initialize();
        LOGI("Checking target library %s", libName);
    } while (!Utils::isLibraryLoaded(libName));
    LOGI("Patchx starting modification now...");
    // Modification start here...

    // const MoneyMethod Il2cppClassDef.PlayerClass->getMethod("getMoney", 1);
    // # Please read Il2cpp/Aetherim/README.md for more information.
    // # By this now you can hook Unity Method without offset and less update.
    // # Unity Method basicly have information on sturct Il2cppMethodInfo on Il2cpp Header
    // - So, for this case we will need methodPointer.
    // DobbyHook((void *)MoneyMethod->methodPointer, (void *)hook_getMoney, (void **)&orig_getMoney);

    // NOTE: You can use GlossHook for advanced Patching
    // - You can see that on `<project-path>/include/GlossHook/Gloss.h`.

    // Macros Method //
    // HOOK("0x12345", hook_MakeMeReturnTrue, orig_MakeMeReturnTrue);
    // HOOKSYM("SymMakeMeReturnTrue", hook_MakeMeReturnTrue, orig_MakeMeReturnTrue);

    // Optional Method //
    // DobbyHook(0x12345, (void*)hook_MakeMeReturnTrue, (void **)&orig_MakeMeReturnTrue);
    // DobbyHook((void *)DobbySymbolResolver("libMyTarget.so", "SymMakeMeReturnTrue"), (void *)hook_MakeMeReturnTrue, (void **)&orig_MakeMeReturnTrue);

    // Note: In some cases, you don't really need to hook, just Patching
    // - For example we can use this Hex Patch
    // - 01 00 A0 E3 (MOV R0, #1) Armeabi-v7a
    // - 20 00 80 52 (MOV W0, #1) Aarch64/Arm64-v8a
    // - You need to know about Reverse Engineering at least you need Offset.
    // Lol. just use easy one, right?

    pthread_exit(nullptr);
    return nullptr;
}

__attribute__((constructor)) void lib_main()
{
    pthread_t hacks;
    pthread_create(&hacks, NULL, hack_thread, NULL);
}