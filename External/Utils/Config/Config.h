#pragma once
#include <Windows.h>
#include "ini.h"

struct Globals
{
    // Status
    bool Run = false;
    RECT GameRect;
    POINT GamePoint;
    bool ShowMenu = false;

    // AimBot
    bool AimBot = true;
    bool AimAtTeam = false;
    bool VisCheck = true;
    int AimTargetBone = 1;
    bool DrawFov = true;
    bool RainbowFov = false;
    bool FovFilled = false;
    float AimFov = 150.f;
    int Smooth = 1;
    float Aim_MaxDistance = 150.f;
    int AimType = 0;
    int AimKeyType = 1;

    // Visual
    bool PlayerESP = true;
    bool TeamESP = false;
    bool VehicleESP = true;
    bool ESP_Box = true;
    bool ESP_BoxFilled = false;
    bool ESP_Line = false;
    bool ESP_Distance = true;
    bool ESP_Name = false;
    bool ESP_HealthBar = true;
    bool ESP_Skeleton = true;
    float ESP_MaxDistance = 1000.f;
    int ESP_BoxType = 1;
    int ESP_SkeletonColor = 0;

    // System
    bool StreamProof = false;
    bool Crosshair = false;
    int CrosshairType = 0;
    int RainbowRate = 25;

    // Misc
    bool SpectatorList = true;
    bool UnlockAll = false;
    bool SwayModify = false;
    float ModVal = 0.95f;
    bool DamageHack = false;
    int ModBPS = 1;
    bool FovChanger = false;
    bool ForceUserFov = false;
    float NormalFOV = 0.f;
    float ViewModelFOV = 0.f;

    // Key
    int MenuKey = VK_INSERT;
    int AimKey0 = VK_RBUTTON;
    int AimKey1 = VK_LBUTTON;

    // KeyBinder
    bool KeyBinding = false;
    int BindingID = 0;
};

// Using mINI
// https://github.com/metayeti/mINI
class ConfigManager
{
private:
    mINI::INIStructure ini;
public:
    bool CheckDir(char* dir);
    void GenerateFile(std::string filename);
    void DeleteConfig(std::string filename);
    void SaveSetting(std::string filename);
    void LoadSetting(std::string filename);
    std::vector<std::string> GetFileList(const std::string& directoryPath);
};

extern Globals g;
extern bool IsKeyDown(int VK);