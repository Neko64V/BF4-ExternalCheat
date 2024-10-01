#include "Config.h"
#include <thread>
#include <filesystem>

Globals g;

bool IsKeyDown(int VK)
{
    return (GetAsyncKeyState(VK) & 0x8000) != 0;
}

bool ConfigManager::CheckDir(char* dir)
{
    return std::filesystem::exists(dir) && std::filesystem::is_directory(dir);
}

void ConfigManager::GenerateFile(std::string filename)
{
    if (filename.empty())
        return;

    std::string path = ".\\Config\\" + filename;

    if (filename.find(".ini") == std::string::npos)
        path += ".ini";

    mINI::INIFile file(path.c_str());

    // config.iniの存在をチェック
    if (!std::filesystem::is_regular_file(path.c_str()))
        file.generate(ini, true);
}

void ConfigManager::DeleteConfig(std::string filename)
{
    std::string tmp = ".\\Config\\" + filename;

    std::filesystem::remove(tmp.c_str());
}

std::vector<std::string> ConfigManager::GetFileList(const std::string& directoryPath)
{
    std::vector<std::string> iniFileList;

    for (const auto& entry : std::filesystem::directory_iterator(directoryPath))
    {
        if (entry.is_regular_file())
        {
            auto path = entry.path();
            if (path.extension() == ".ini")
                iniFileList.push_back(path.filename().string());
        }
    }

    return iniFileList;
}

void ConfigManager::SaveSetting(std::string filename)
{
    if (filename.empty())
        return;

    std::string path = ".\\Config\\" + filename;

    if (filename.find(".ini") == std::string::npos)
        path += ".ini";

    mINI::INIFile file(path.c_str());

    // config.iniの存在をチェック
    if (!std::filesystem::is_regular_file(path.c_str()))
        file.generate(ini, true);
    else
        file.read(ini);

    // AimBot
    ini["aimbot"]["enable"] = std::to_string(g.AimBot).c_str();
    ini["aimbot"]["aimatteam"] = std::to_string(g.AimAtTeam).c_str();
    ini["aimbot"]["vischeck"] = std::to_string(g.VisCheck).c_str();
    ini["aimbot"]["targetbone"] = std::to_string(g.AimTargetBone).c_str();
    ini["aimbot"]["drawfov"] = std::to_string(g.DrawFov).c_str();
    ini["aimbot"]["rainbowfov"] = std::to_string(g.RainbowFov).c_str();
    ini["aimbot"]["fovfilled"] = std::to_string(g.FovFilled).c_str();
    ini["aimbot"]["fov"] = std::to_string(g.AimFov).c_str();
    ini["aimbot"]["smooth"] = std::to_string(g.Smooth).c_str();
    ini["aimbot"]["distance"] = std::to_string(g.Aim_MaxDistance).c_str();
    ini["aimbot"]["aimtype"] = std::to_string(g.AimType).c_str();
    ini["aimbot"]["aimkeytyle"] = std::to_string(g.AimKeyType).c_str();

    // Visual
    ini["visual"]["enable"] = std::to_string(g.PlayerESP).c_str();
    ini["visual"]["team"] = std::to_string(g.TeamESP).c_str();
    ini["visual"]["vehicle"] = std::to_string(g.VehicleESP).c_str();
    ini["visual"]["box"] = std::to_string(g.ESP_Box).c_str();
    ini["visual"]["boxfilled"] = std::to_string(g.ESP_BoxFilled).c_str();
    ini["visual"]["line"] = std::to_string(g.ESP_Line).c_str();
    ini["visual"]["distance"] = std::to_string(g.ESP_Distance).c_str();
    ini["visual"]["name"] = std::to_string(g.ESP_Name).c_str();
    ini["visual"]["healthbar"] = std::to_string(g.ESP_HealthBar).c_str();
    ini["visual"]["skeleton"] = std::to_string(g.ESP_Skeleton).c_str();
    ini["visual"]["maxdistance"] = std::to_string(g.ESP_MaxDistance).c_str();
    ini["visual"]["boxtype"] = std::to_string(g.ESP_BoxType).c_str();

    // Misc
    ini["misc"]["streamproof"] = std::to_string(g.StreamProof).c_str();
    ini["misc"]["crosshair"] = std::to_string(g.Crosshair).c_str();
    ini["misc"]["crosshairtype"] = std::to_string(g.CrosshairType).c_str();
    ini["misc"]["rainbowrate"] = std::to_string(g.RainbowRate).c_str();
    ini["misc"]["unlockall"] = std::to_string(g.UnlockAll).c_str();
    ini["misc"]["swaymod"] = std::to_string(g.SwayModify).c_str();
    ini["misc"]["swayvalue"] = std::to_string(g.ModVal).c_str();
    ini["misc"]["damagehack"] = std::to_string(g.DamageHack).c_str();
    ini["misc"]["bps"] = std::to_string(g.ModBPS).c_str();

    // Keys
    ini["key"]["aim0"] = std::to_string(g.AimKey0).c_str();
    ini["key"]["aim1"] = std::to_string(g.AimKey1).c_str();

    file.write(ini);
}

void ConfigManager::LoadSetting(std::string filename)
{
    if (filename.empty())
        return;

    std::string path = ".\\Config\\" + filename;
    mINI::INIFile file(path.c_str());

    // config.iniの存在をチェック
    if (!std::filesystem::is_regular_file(path.c_str()))
        file.generate(ini, true);
    else
        file.read(ini);

    // AimBot
    std::istringstream(ini["aimbot"]["enable"]) >> g.AimBot;
    std::istringstream(ini["aimbot"]["aimatteam"]) >> g.AimAtTeam;
    std::istringstream(ini["aimbot"]["vischeck"]) >> g.VisCheck;
    std::istringstream(ini["aimbot"]["targetbone"]) >> g.AimTargetBone;
    std::istringstream(ini["aimbot"]["drawfov"]) >> g.DrawFov;
    std::istringstream(ini["aimbot"]["rainbowfov"]) >> g.RainbowFov;
    std::istringstream(ini["aimbot"]["fovfilled"]) >> g.FovFilled;
    std::istringstream(ini["aimbot"]["fov"]) >> g.AimFov;
    std::istringstream(ini["aimbot"]["smooth"]) >> g.Smooth;
    std::istringstream(ini["aimbot"]["distance"]) >> g.Aim_MaxDistance;
    std::istringstream(ini["aimbot"]["aimtype"]) >> g.AimType;
    std::istringstream(ini["aimbot"]["aimkeytyle"]) >> g.AimKeyType;

    // Visual
    std::istringstream(ini["visual"]["enable"]) >> g.PlayerESP;
    std::istringstream(ini["visual"]["team"]) >> g.TeamESP;
    std::istringstream(ini["visual"]["vehicle"]) >> g.VehicleESP;
    std::istringstream(ini["visual"]["box"]) >> g.ESP_Box;
    std::istringstream(ini["visual"]["boxfilled"]) >> g.ESP_BoxFilled;
    std::istringstream(ini["visual"]["line"]) >> g.ESP_Line;
    std::istringstream(ini["visual"]["distance"]) >> g.ESP_Distance;
    std::istringstream(ini["visual"]["name"]) >> g.ESP_Name;
    std::istringstream(ini["visual"]["healthbar"]) >> g.ESP_HealthBar;
    std::istringstream(ini["visual"]["skeleton"]) >> g.ESP_Skeleton;
    std::istringstream(ini["visual"]["maxdistance"]) >> g.ESP_MaxDistance;
    std::istringstream(ini["visual"]["boxtype"]) >> g.ESP_BoxType;

    // System
    std::istringstream(ini["system"]["streamproof"]) >> g.StreamProof;
    std::istringstream(ini["misc"]["crosshair"]) >> g.Crosshair;
    std::istringstream(ini["misc"]["crosshairtype"]) >> g.CrosshairType;
    std::istringstream(ini["misc"]["rainbowrate"]) >> g.RainbowRate;
    std::istringstream(ini["misc"]["unlockall"]) >> g.UnlockAll;
    std::istringstream(ini["misc"]["swaymod"]) >> g.SwayModify;
    std::istringstream(ini["misc"]["swayvalue"]) >> g.ModVal;
    std::istringstream(ini["misc"]["damagehack"]) >> g.DamageHack;
    std::istringstream(ini["misc"]["bps"]) >> g.ModBPS;

    // Keys
    std::istringstream(ini["key"]["aim0"]) >> g.AimKey0;
    std::istringstream(ini["key"]["aim1"]) >> g.AimKey1;
}