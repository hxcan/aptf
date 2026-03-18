import qbs

CppApplication
{
    name: 'aptf'

    files: [
        "AptF.cpp",
        "AptF.h",
        "AptF.pro",
        "AptF.ui",
        "AptfSettingDialog.cpp",
        "AptfSettingDialog.h",
        "AptfSettingDialog.ui",
        "AptfTtsClient.cpp",
        "AptfTtsClient.h",
        "PlainTextEditMouseRelease.cpp",
        "PlainTextEditMouseRelease.h",
        "main.cpp",
    ]

    Depends
    {
        name: "Qt"
        submodules:
            [
            "gui",
            "widgets",
            "network",
        ]
    }
    Properties {
        condition: !qbs.targetOS.contains("macos")
        cpp.dynamicLibraries:
            [
            "toast",
        ]
    }


}



