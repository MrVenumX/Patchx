#include "Il2cpp.hpp"

Il2cppWrapper::~Il2cppWrapper()
{
}

Il2cppWrapper::initialize()
{
    LOGI("[Aetherim] libil2cpp.so found, initialize Aetherim");
    Il2cppApi::initialize();
    LOGI("[Aetherim] Initialize completed");

    const auto Aetherim = std::make_unique<Wrapper>();

    LOGI("[Aetherim] Image count : %i", (int)Aetherim->getImages().size());
    LOGI("[Aetherim] Class count : %i", (int)Aetherim->getImage("Assembly-CSharp.dll")->getClasses().size());

    Il2cppImageDef.AssemblyImage = Aetherim->getImage("Assembly-CSharp.dll");
    Il2cppImageDef.UnityEngineImage = Aetherim->getImage("UnityEngine.dll");

    Il2cppClassDef.ApplicationClass = Il2cppImageDef.UnityEngineImage->getClass("Application", "UnityEngine");
    Il2cppClassDef.ScreenClass = Il2cppImageDef.UnityEngineImage->getClass("Screen", "UnityEngine");
    Il2cppClassDef.CameraClass = Il2cppImageDef.UnityEngineImage->getClass("Camera", "UnityEngine");
    Il2cppClassDef.GameObjetClass = Il2cppImageDef.UnityEngineImage->getClass("GameObject", "UnityEngine");
    Il2cppClassDef.ObjectClass = Il2cppImageDef.UnityEngineImage->getClass("Object", "UnityEngine");
    Il2cppClassDef.ComponentClass = Il2cppImageDef.UnityEngineImage->getClass("Component", "UnityEngine");
    Il2cppClassDef.TransformClass = Il2cppImageDef.UnityEngineImage->getClass("Transform", "UnityEngine");
    Il2cppClassDef.PhysicsClass = Il2cppImageDef.UnityEngineImage->getClass("Physics", "UnityEngine");
}