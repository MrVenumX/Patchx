#pragma once

#include "Includes/Utils.h"
#include "./Aetherim/wrapper.hpp"

struct Il2cppImageDef
{
    const Il2cppImage *AssemblyImage;
    const Il2cppImage *UnityEngineImage;
} Il2cppImageDef;

struct Il2cppClassDef
{
    // Please declarate your class from dump.cs

    // const Il2cppClass *PlayerClass;

    // UnityEngine.dll, Don't delete this!
    const Il2cppClass *ApplicationClass;
    const Il2cppClass *ScreenClass;
    const Il2cppClass *CameraClass;
    const Il2cppClass *GameObjetClass;
    const Il2cppClass *ObjectClass;
    const Il2cppClass *ComponentClass;
    const Il2cppClass *TransformClass;
    const Il2cppClass *PhysicsClass;
} Il2cppClassDef;

namespace Il2cppWrapper
{
    void initialize()
    {
        LOGI("[Aetherim] libil2cpp.so found, initialize Aetherim");
        Il2cppApi::initialize();
        LOGI("[Aetherim] Initialize completed");

        const auto Aetherim = std::make_unique<Wrapper>();

        LOGI("[Aetherim] Image count : %i", (int)Aetherim->getImages().size());
        LOGI("[Aetherim] Class count : %i", (int)Aetherim->getImage("Assembly-CSharp.dll")->getClasses().size());

        Il2cppImageDef.AssemblyImage = Aetherim->getImage("Assembly-CSharp.dll");
        Il2cppImageDef.UnityEngineImage = Aetherim->getImage("UnityEngine.dll");

        // Please do like this example.
        // # if class have namespace do like this `getClass("Player", "PlayerNamespace");`
        // # By default, if getClass have no namespace arguments getClass will auto declarate as empty.
        // Il2cppClassDef.PlayerClass = Il2cppImageDef.AssemblyImage->getClass("Player");

        // UnityEngine.dll, Don't delete this!
        Il2cppClassDef.ApplicationClass = Il2cppImageDef.UnityEngineImage->getClass("Application", "UnityEngine");
        Il2cppClassDef.ScreenClass = Il2cppImageDef.UnityEngineImage->getClass("Screen", "UnityEngine");
        Il2cppClassDef.CameraClass = Il2cppImageDef.UnityEngineImage->getClass("Camera", "UnityEngine");
        Il2cppClassDef.GameObjetClass = Il2cppImageDef.UnityEngineImage->getClass("GameObject", "UnityEngine");
        Il2cppClassDef.ObjectClass = Il2cppImageDef.UnityEngineImage->getClass("Object", "UnityEngine");
        Il2cppClassDef.ComponentClass = Il2cppImageDef.UnityEngineImage->getClass("Component", "UnityEngine");
        Il2cppClassDef.TransformClass = Il2cppImageDef.UnityEngineImage->getClass("Transform", "UnityEngine");
        Il2cppClassDef.PhysicsClass = Il2cppImageDef.UnityEngineImage->getClass("Physics", "UnityEngine");
    }
}
