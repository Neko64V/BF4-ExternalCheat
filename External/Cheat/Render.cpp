#include "Cheat.h"
#include "..\Overlay\Overlay.h"
#include "..\ImGui\Font\RobotoRegular.h"
#include "..\ImGui\Font\font_awesome.h"
#include <filesystem>

// Menu String
const char* BoxList[] = { "2D Box", "2D Corner Box" };
const char* AimTargetList[] = { "Head", "Spine" };
const char* AimTypeList[] = { "Crosshair", "Distance" };
const char* AimKeyTypeList[] = { "and", "or" };
const char* SkeletonColorList[] = { "ESP", "User" };
const char* CrosshairList[] = { "Cross", "Circle" };

// Config
ConfigManager Config;
char ConfigPath[] = ".\\Config\\";

void SliderEx(const char* Title, const char* SliderText, const char* Format, float& Value, float Min, float Max);
void SliderEx(const char* Title, const char* SliderText, const char* Format, int& Value, int Min, int Max);
std::vector<std::string> IconList = { "\n\xef\x81\x9b", "\n\xef\x81\xae", "\n\xef\x84\xa1", "\xef\x85\x9b" };

// false��Ԃ��Ȃ��̂ł��[��A
bool Cheat::Init() 
{
    // LoadFont
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.Fonts->AddFontFromMemoryCompressedTTF(RobotoRegular_compressed_data, RobotoRegular_compressed_size, 14.f, nullptr);

    // LoadIcon
    const ImWchar icon_range[]{ ICON_MIN_FA, ICON_MAX_FA, 0 };
    ImFontConfig icon_cfg;
    icon_cfg.MergeMode = true;
    icon_cfg.PixelSnapH = true;
    icon_cfg.OversampleH = 3;
    icon_cfg.OversampleV = 3;
    io.Fonts->AddFontFromMemoryCompressedTTF(font_awesome_data, font_awesome_size, 30.f, &icon_cfg, icon_range);
    io.Fonts->Build();

    // Config�t�H���_�̑��݂�`�F�b�N
    if (!Config.CheckDir(ConfigPath))
        system("mkdir Config");

    GameWindow = FindWindowA(nullptr, "Battlefield 4");

    return true;
}

void Cheat::RenderSpectatorList()
{
    static bool first = false;
    if (!first)
    {
        ImGui::SetNextWindowPos(ImVec2(8.f, 24.f));
        ImGui::SetNextWindowSize(ImVec2(200.f, 110.f));
        first = !first;
    }
    
    ImGui::SetNextWindowBgAlpha(0.8f);
    ImGui::Begin("SpectatorList", (bool*)NULL);

    for (auto Spectator : SpectatorNameList)
        ImGui::Text("%s", Spectator.c_str());
   
    ImGui::End();
}

void Cheat::RenderInfo()
{
    // FPS��
    std::string framerate = std::to_string((int)ImGui::GetIO().Framerate) + " FPS";
    ImGui::GetBackgroundDrawList()->AddText(ImVec2(8.f, 8.f), ImColor(1.f, 1.f, 1.f, 1.f), framerate.c_str());

    // Spectator
    if (SpectatorNameList.size() != 0)
        ImGui::GetBackgroundDrawList()->AddText(ImVec2((g.GameRect.right / 2.f) - ImGui::CalcTextSize("[ SpectatorFound! ]").x, g.GameRect.top + 2.f), ImColor(1.f, 0.f, 0.f, 1.f), "[ SpectatorFound! ]");
    
    // AimFov
    if (g.AimBot && g.DrawFov)
    {
        ImColor fov_color = g.RainbowFov ? Rainbow : FOV_User;

        ImGui::GetBackgroundDrawList()->AddCircle(ImVec2((float)g.GameRect.right / 2.f, (float)g.GameRect.bottom / 2.f), g.AimFov, fov_color, 0.f);

        if (g.FovFilled)
            ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2((float)g.GameRect.right / 2.f, (float)g.GameRect.bottom / 2.f), g.AimFov, ImColor(fov_color.Value.x, fov_color.Value.y, fov_color.Value.z, 0.1f), NULL);
    }

    // Crosshair
    if (g.Crosshair)
    {
        switch (g.CrosshairType)
        {
        case 0:
            ImGui::GetBackgroundDrawList()->AddLine(ImVec2(((float)g.GameRect.right / 2.f + 4), ((float)g.GameRect.bottom / 2.f)), ImVec2(((float)g.GameRect.right / 2.f + 10), ((float)g.GameRect.bottom / 2.f)), ImColor(1.f, 1.f, 1.f, 1.f), 1);
            ImGui::GetBackgroundDrawList()->AddLine(ImVec2(((float)g.GameRect.right / 2.f - 4), ((float)g.GameRect.bottom / 2.f)), ImVec2(((float)g.GameRect.right / 2.f - 10), ((float)g.GameRect.bottom / 2.f)), ImColor(1.f, 1.f, 1.f, 1.f), 1);
            ImGui::GetBackgroundDrawList()->AddLine(ImVec2(((float)g.GameRect.right / 2.f), ((float)g.GameRect.bottom / 2.f + 4)), ImVec2(((float)g.GameRect.right / 2.f), ((float)g.GameRect.bottom / 2.f + 10)), ImColor(1.f, 1.f, 1.f, 1.f), 1);
            ImGui::GetBackgroundDrawList()->AddLine(ImVec2(((float)g.GameRect.right / 2.f), ((float)g.GameRect.bottom / 2.f - 4)), ImVec2(((float)g.GameRect.right / 2.f), ((float)g.GameRect.bottom / 2.f - 10)), ImColor(1.f, 1.f, 1.f, 1.f), 1);
            break;
        case 1:
            ImGui::GetBackgroundDrawList()->AddCircle(ImVec2((float)g.GameRect.right / 2.f, (float)g.GameRect.bottom / 2.f), 3, ImColor(1.f, 1.f, 1.f, 1.f), NULL);
            break;
        default:
            break;
        }
    }
}

void Cheat::RenderMenu()
{
    // Setups
    ImVec4* colors = ImGui::GetStyle().Colors;
    ImGuiStyle& style = ImGui::GetStyle();
    auto DefaultItemSpacing = style.ItemSpacing;
    const char* text1 = g.KeyBinding && g.BindingID == 1 ? "< Press Any Key >" : KeyNames[g.AimKey0];
    const char* text2 = g.KeyBinding && g.BindingID == 2 ? "< Press Any Key >" : KeyNames[g.AimKey1];
    static int Index = 0;
    static int FileNum = 0;
    static char InputName[12];
    static bool DeleteFlag = false;
    auto vec = Config.GetFileList(ConfigPath);
    const char** FileList = new const char* [vec.size()];

    for (size_t j = 0; j < vec.size(); j++)
        FileList[j] = vec[j].c_str();

    ImGui::SetNextWindowSize(ImVec2(675.f, 450.f));
    ImGui::Begin("Battlefield 4 [ EXTERNAL ]", &g.ShowMenu, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    //---// Clild 0 //-----------------------------------//
    ImGui::BeginChild("##BaseChild", ImVec2(100.f, ImGui::GetContentRegionAvail().y), false);

    ImVec4 DefaultButtonCol = colors[ImGuiCol_Button];

    for (int i = 0; i < IconList.size(); i++)
    {
        colors[ImGuiCol_Button] = Index == i ? ImVec4(0.23f, 0.23f, 0.23f, 1.f) : colors[ImGuiCol_WindowBg];

        if (ImGui::Button(IconList[i].c_str(), ImVec2(100.f, 100.f)))
            Index = i;

        colors[ImGuiCol_Button] = DefaultButtonCol;
    }

    ImGui::EndChild();
    //---// Clild 0 //-----------------------------------//

    ImGui::SameLine();

    //---// Clild 1 //-----------------------------------//
    ImGui::BeginChild("BaseChild##2", ImVec2(ImGui::GetContentRegionAvail()));

    switch (Index)
    {
    case 0:
        ImGui::Spacing();
        ImGui::BeginChild("##LeftAimBase", ImVec2(ImGui::GetContentRegionAvail().x / 2.f - 8.f, ImGui::GetContentRegionAvail().y), false);

        ImGui::Text("  AimBot");
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Checkbox("AimBot", &g.AimBot);
        ImGui::Checkbox("Aim at Team", &g.AimAtTeam);

        ImGui::NewLine();
        ImGui::Spacing();

        ImGui::Text("  AimBot Config");
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Checkbox("Visibility Check", &g.VisCheck);
        ImGui::Combo("AimBone", &g.AimTargetBone, AimTargetList, IM_ARRAYSIZE(AimTargetList));

        ImGui::NewLine();
        ImGui::Spacing();

        ImGui::Text("  FOV");
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Checkbox("Draw FOV", &g.DrawFov);
        ImGui::Checkbox("Rainbow FOV", &g.RainbowFov);
        ImGui::Checkbox("Fov Filled", &g.FovFilled);
        SliderEx("FOV", "##AimFOV", "%.f", g.AimFov, 100.f, 600.f);
        ImGui::ColorEdit4("FOV Color", &FOV_User.Value.x);

        ImGui::EndChild();
        /*---------------*/
        ImGui::SameLine();
        /*---------------*/
        ImGui::BeginChild("##RightAimBase", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), false);

        ImGui::Text("  AimBot Setting");
        ImGui::Separator();
        ImGui::Spacing();
        style.ItemSpacing.y = 1.f;
        SliderEx("FOV", "##AimFOV", "%.f", g.AimFov, 100.f, 600.f);
        SliderEx("Smooth", "##AimSmooth", "%d", g.Smooth, 1, 20);
        SliderEx("Distance", "##AimDistance", "%.fm", g.Aim_MaxDistance, 10.f, 100.f);
        style.ItemSpacing = DefaultItemSpacing;

        ImGui::Spacing();

        ImGui::Combo("AimType", &g.AimType, AimTypeList, IM_ARRAYSIZE(AimTypeList));

        ImGui::NewLine();
        ImGui::Spacing();

        ImGui::Text("  KeyBind");
        ImGui::Separator();
        ImGui::Spacing();

        if (ImGui::Button(text1, ImVec2(215.f, 20.f)))
        {
            g.KeyBinding = true;
            g.BindingID = 1;
            std::thread([&]() {this->KeyBinder(g.AimKey0); }).detach();
        }

        ImGui::PushItemWidth(215.f);
        ImGui::Combo("##KeyType", &g.AimKeyType, AimKeyTypeList, IM_ARRAYSIZE(AimKeyTypeList));
        ImGui::PopItemWidth();

        if (ImGui::Button(text2, ImVec2(215.f, 20.f)))
        {
            g.KeyBinding = true;
            g.BindingID = 2;
            std::thread([&]() {this->KeyBinder(g.AimKey1); }).detach();
        }

        ImGui::EndChild();
        break;
    case 1:
        ImGui::Spacing();
        ImGui::BeginChild("##LeftVisualBase", ImVec2(ImGui::GetContentRegionAvail().x / 2.f - 8.f, ImGui::GetContentRegionAvail().y), false);

        ImGui::Text("  Visual");
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Checkbox("Player ESP", &g.PlayerESP);
        ImGui::Checkbox("Team ESP", &g.TeamESP);
        ImGui::Checkbox("Vehicle ESP", &g.VehicleESP);

        ImGui::NewLine();
        ImGui::Spacing();

        ImGui::Text("  ESP Options");
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Checkbox("Box", &g.ESP_Box);
        ImGui::Checkbox("BoxFilled", &g.ESP_BoxFilled);
        ImGui::Checkbox("Line", &g.ESP_Line);
        ImGui::Checkbox("Skeleton", &g.ESP_Skeleton);
        ImGui::Checkbox("Distance", &g.ESP_Distance);
        ImGui::Checkbox("Name", &g.ESP_Name);
        ImGui::Checkbox("HealthBar", &g.ESP_HealthBar);

        ImGui::EndChild();
        /*---------------*/
        ImGui::SameLine();
        /*---------------*/
        ImGui::BeginChild("##RightVisualBase", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), false);

        ImGui::Text("  ESP Setting");
        ImGui::Separator();
        ImGui::Spacing();

        SliderEx("Distance", "##ESP-Distance", "%.fm", g.ESP_MaxDistance, 100.f, 2000.f);
        ImGui::Combo("Box Style", &g.ESP_BoxType, BoxList, IM_ARRAYSIZE(BoxList));

        ImGui::NewLine();
        ImGui::Spacing();

        ImGui::Text("  ESP Colors");
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::ColorEdit4("Normal", &ESP_Normal.Value.x);
        ImGui::ColorEdit4("Visible", &ESP_Visible.Value.x);
        ImGui::ColorEdit4("Team", &ESP_Team.Value.x);
        ImGui::ColorEdit4("Vehicle", &ESP_Vehicle.Value.x);
        ImGui::ColorEdit4("BoxFilled", &ESP_Filled.Value.x);
        ImGui::ColorEdit4("Skeleton", &ESP_Skeleton.Value.x);
        ImGui::Combo("Skeleton Color", &g.ESP_SkeletonColor, SkeletonColorList, IM_ARRAYSIZE(SkeletonColorList));

        ImGui::EndChild();
        /*---------------*/
        break;
    case 2:
        ImGui::Spacing();
        ImGui::BeginChild("##LeftMiscBase", ImVec2(ImGui::GetContentRegionAvail().x / 2.f - 12.f, ImGui::GetContentRegionAvail().y), false);

        ImGui::Text("  System");
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Checkbox("SpectatorList", &g.SpectatorList);
        ImGui::Checkbox("StreamProof", &g.StreamProof);
        ImGui::Checkbox("Crosshair", &g.Crosshair);
        ImGui::Combo("Type", &g.CrosshairType, CrosshairList, IM_ARRAYSIZE(CrosshairList));
        SliderEx("RainbowRate", "##RainbowRT", "%d", g.RainbowRate, 1, 200);

        ImGui::Text("  Game");
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Checkbox("SwayModify", &g.SwayModify);
        ImGui::Checkbox("DamageHack", &g.DamageHack); ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "[Risk]");
        ImGui::Checkbox("FOV Changer", &g.FovChanger);
        ImGui::Checkbox("Force UserFOV", &g.ForceUserFov);

        ImGui::EndChild();
        /*---------------*/
        ImGui::SameLine();
        /*---------------*/
        ImGui::BeginChild("##RightMiscBase", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), false);

        ImGui::Text("   SwayModify");
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Checkbox("SwayModify", &g.SwayModify);

        ImGui::Spacing(); ImGui::SameLine();
        ImGui::SliderFloat("##SwayMod", &g.ModVal, 0.f, 1.f); ImGui::SameLine();
        ImGui::Text("%.f%%", g.ModVal * 100.f);

        ImGui::NewLine();
        ImGui::Spacing();

        ImGui::Text("   DamageHack");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "[Risk]");
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Checkbox("DamageHack", &g.DamageHack);

        ImGui::Spacing(); ImGui::SameLine();
        ImGui::SliderInt("##BulletPerShell", &g.ModBPS, 1, 30); ImGui::SameLine();
        ImGui::Text("%d", g.ModBPS);

        ImGui::Text("CurrentBPS : %d", CurrentBPS);
        if (ImGui::Button("Apply##BPS"))
            std::thread([&]() { this->SetBPS(g.ModBPS); }).detach();

        ImGui::NewLine();
        ImGui::Spacing();

        ImGui::Text("   FOV Changer");
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Checkbox("FOV Changer", &g.FovChanger);
        ImGui::Checkbox("Force UserFOV", &g.ForceUserFov);

        style.ItemSpacing.y = 1.f;
        SliderEx("Normal FOV", "##FOV_Normal", "%.f", g.NormalFOV, 50.f, 120.f);
        SliderEx("ViewModel FOV", "##FOV_ViewModel", "%.f", g.ViewModelFOV, 50.f, 120.f);
        style.ItemSpacing = DefaultItemSpacing;

        if (ImGui::Button("Apply", ImVec2(75.f, 20.f)))
            std::thread([&]() { this->SetFOV(g.NormalFOV, g.ViewModelFOV); }).detach();

        ImGui::SameLine();

        if (ImGui::Button("Set Default", ImVec2(75.f, 20.f)))
        {
            g.ForceUserFov = false;
            std::thread([&]() { this->SetDefaultFOV(); }).detach();
            g.NormalFOV = this->DefaultFov;
            g.ViewModelFOV = this->DefaultViewModelFov;
        }

        ImGui::EndChild();
        /*---------------*/
        break;
    case 3:
        ImGui::Spacing();
        ImGui::BeginChild("##LeftDevBase", ImVec2(ImGui::GetContentRegionAvail().x / 2.f - 8.f, ImGui::GetContentRegionAvail().y), false);

        ImGui::Text("  Config");
        ImGui::Separator();
        ImGui::Spacing();

        for (size_t j = 0; j < vec.size(); j++)
            FileList[j] = vec[j].c_str();

        ImGui::InputText("Name", InputName, IM_ARRAYSIZE(InputName));
        if (ImGui::Button("Generate ConfigFile", ImVec2(ImGui::GetContentRegionAvail().x, 20.f)))
        {
            std::thread([&]() {Config.GenerateFile(InputName); }).join();
            ZeroMemory(InputName, sizeof(InputName));
        }

        ImGui::Spacing();
        ImGui::Spacing();

        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::ListBox("##List", &FileNum, FileList, vec.size());

        // Button
        if (ImGui::Button("Save", ImVec2(ImGui::GetContentRegionAvail().x / 3.f - 4.f, 20.f)))
            std::thread([&]() {Config.SaveSetting(InputName[0] != NULL ? InputName : FileList[FileNum]); }).join();
        ImGui::SameLine();
        if (ImGui::Button("Load", ImVec2(ImGui::GetContentRegionAvail().x / 2.f - 4.f, 20.f)) && vec.size() != 0)
            std::thread([&]() {Config.LoadSetting(FileList[FileNum]); }).join();
        ImGui::SameLine();
        if (ImGui::Button("Delete", ImVec2(ImGui::GetContentRegionAvail().x, 20.f)) && vec.size() != 0)
            DeleteFlag = true;

        if (DeleteFlag)
        {
            ImGui::Text("Delete this file?");

            if (ImGui::Button("OK", ImVec2(90.f, 20.f)))
            {
                std::thread([&]() {Config.DeleteConfig(FileList[FileNum]); }).join();
                DeleteFlag = false;
            }

            ImGui::SameLine();

            if (ImGui::Button("Cancel", ImVec2(90.f, 20.f)))
                DeleteFlag = false;
        }

        delete[] FileList;

        ImGui::EndChild();
        /*---------------*/
        ImGui::SameLine();
        /*---------------*/
        ImGui::BeginChild("##RightDevBase", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), false);

        ImGui::Text("  Process");
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Text("PID : %d", m.PID);

        ImGui::NewLine();
        ImGui::Spacing();

        ImGui::Text("  Contact Developer");
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Text("Twitter : @WallHax_Ena");
        ImGui::Text("Discord : wallhax_ena");
        ImGui::NewLine();
        if (ImGui::Button("Open Twitter"))
            ShellExecuteA(NULL, "open", "https://twitter.com/WallHax_Ena", NULL, NULL, SW_SHOWDEFAULT);

        // Exit
        ImGui::NewLine();
        ImGui::Separator();
        ImGui::Spacing();
        if (ImGui::Button("Exit", ImVec2(ImGui::GetContentRegionAvail().x, 30.f)))
            g.Run = false;

        ImGui::EndChild();
        /*---------------*/
        break;
    default:
        break;
    }

    ImGui::EndChild();
    ImGui::End();
}

void Cheat::RenderESP()
{
    CPlayer player, * pEntity = &player;
    CPlayer local, * pLocal = &local;

    // Context
    uint64_t ClientGameContext = m.Read<uint64_t>(offset::ClientgameContext);
    uint64_t PlayerManager = m.Read<uint64_t>(ClientGameContext + offset::PlayerManager);
    uint64_t PlayerEntity = m.Read<uint64_t>(PlayerManager + offset::ClientPlayer);

    // Aim
    float FOV = 0.f;
    float MinFov = 999.f;
    float MinDistance = 9999.f;
    Vector2 ScreenMiddle = { (float)g.GameRect.right / 2.f, (float)g.GameRect.bottom / 2.f };
    Vector2 TargetPosition = Vector2(0.f, 0.f);

    // LocalPlayer
    uintptr_t localptr = m.Read<uint64_t>(PlayerManager + offset::LocalPlayer);
    pLocal->GetPlayer(localptr);
    pLocal->Update();

    if (pLocal->IsDead() && !pLocal->InVehicle())
        return;

    UpdateW2SData();
    SpectatorNameList.clear();

    // ESP Loop
    for (int i = 0; i < 72; i++)
    {
        // Update Player
        uintptr_t player = m.Read<uint64_t>(PlayerEntity + (i * 0x8));
        pEntity->GetPlayer(player);
        pEntity->Update();

        // Spectaror Warning
        if (pEntity->IsSpectator())
        {
            SpectatorNameList.push_back(pEntity->Name);
            continue;
        }

        // Invalid Player
        if (pEntity->ClientPlayer == NULL)
            continue;
        else if (pEntity->ClientPlayer == pLocal->ClientPlayer)
            continue;
        else if (pEntity->ClientVehicle == pLocal->ClientVehicle && pLocal->ClientVehicle != 0 && pEntity->ClientVehicle != 0)
            continue;

        // GetDistance
        float distance = GetDistance(pLocal->Position, pEntity->Position);

        // Check ESP MaxDistance
        if (g.ESP_MaxDistance < distance)
            continue;

        // WorldToScreen
        Vector2 g_Screen{};
        if (!WorldToScreen(pEntity->Position, g_Screen))
            continue;

        // Vehicle ESP
        if (g.VehicleESP && pEntity->InVehicle())
        {
            if (!g.TeamESP && pLocal->Team == pEntity->Team)
                continue;

            Vector3 vTopGamePos = Vector3(pEntity->VehicleAABB.Max.x, pEntity->VehicleAABB.Max.y, pEntity->VehicleAABB.Max.z) + pEntity->Position;
            Vector3 vBottomGamePos = Vector3(pEntity->VehicleAABB.Min.x, pEntity->VehicleAABB.Min.y, pEntity->VehicleAABB.Min.z) + pEntity->Position;

            Vector2 vTop{}, vBottom{};
            if (!WorldToScreen(vTopGamePos, vTop) || !WorldToScreen(vBottomGamePos, vBottom))
                continue;

            ImColor vColor = pEntity->Team == pLocal->Team ? ESP_Team : ESP_Vehicle;
            float vHeight = vBottom.y - vTop.y;
            float vWidth = vHeight / 1.25f;

            if (vHeight < 0.f)
                vHeight *= -1;

            // Line
            if (g.ESP_Line)
                DrawLine(ImVec2(g.GameRect.right / 2.f, g.GameRect.bottom), ImVec2(g_Screen.x, g_Screen.y), vColor, 1.f);

            // 2D Box
            if (g.ESP_Box)
            {
                DrawLine(ImVec2(g_Screen.x + vWidth, g_Screen.y - vHeight), ImVec2(g_Screen.x + vWidth, g_Screen.y), vColor, 1.f);
                DrawLine(ImVec2(g_Screen.x + vWidth, g_Screen.y - vHeight), ImVec2(g_Screen.x - vWidth, g_Screen.y - vHeight), vColor, 1.f);
                DrawLine(ImVec2(g_Screen.x + vWidth, g_Screen.y), ImVec2(g_Screen.x - vWidth, g_Screen.y), vColor, 1.f);
                DrawLine(ImVec2(g_Screen.x - vWidth, g_Screen.y - vHeight), ImVec2(g_Screen.x - vWidth, g_Screen.y), vColor, 1.f);
            }

            // Vehicle Owner(== Driver) Check
            if (pEntity->hComponent.m_Health != 0)
                continue;

            // HealthBar
            if (g.ESP_HealthBar)
            {
                ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(g_Screen.x - vWidth, g_Screen.y + 4.f), ImVec2(g_Screen.x + vWidth, g_Screen.y + 4.f), ImColor(0.f, 0.f, 0.f, 0.8f), 2.f);
                ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(g_Screen.x - vWidth, g_Screen.y + 4.f), ImVec2(g_Screen.x - vWidth + ((vWidth * 2.f) / pEntity->vMaxHealth * pEntity->hComponent.m_VehicleHealth), g_Screen.y + 4.f), ImColor(0.f, 1.f, 0.f, 1.f), 2.f);
            }

            // Distance
            if (g.ESP_Distance)
            {
                std::string vDistStr = std::to_string((int)distance) + "m";
                String(ImVec2(g_Screen.x - (ImGui::CalcTextSize(vDistStr.c_str()).x / 2.f), g_Screen.y + 6.f), ImColor(1.f, 1.f, 1.f, 1.f), vDistStr.c_str());
            }

            // Name
            if (g.ESP_Name)
                String(ImVec2(g_Screen.x - (ImGui::CalcTextSize(pEntity->Name.c_str()).x / 2.f), g_Screen.y - vHeight - 16.f), ImColor(1.f, 1.f, 1.f, 1.f), pEntity->Name.c_str());

            continue;
        }
        else if (pEntity->InVehicle())
        {
            continue;
        }

        // Some check
        if (pEntity->IsDead() || pEntity->InVehicle())
            continue;
        else if (!g.TeamESP && pEntity->Team == pLocal->Team)
            continue;

        // Set ESP color
        ImColor color = pEntity->IsVisible() ? ESP_Visible : ESP_Normal;

        // Teammte
        if (g.TeamESP && pEntity->Team == pLocal->Team)
            color = ESP_Team;

        // Get some size.
        Vector3 AABB_Max = pEntity->Position + pEntity->GetAABB().Max;
        Vector3 AABB_Min = pEntity->Position + pEntity->GetAABB().Min;

        Vector2 pTop, pBottom;
        if (!WorldToScreen(AABB_Max, pTop) || !WorldToScreen(AABB_Min, pBottom))
            continue;
        else if (pTop == Vector2(0.f, 0.f) || pBottom == Vector2(0.f, 0.f))
            continue;

        float Height = pBottom.y - pTop.y;
        float Width = Height / 3.f;

        // Box
        if (g.ESP_Box)
        {
            switch (g.ESP_BoxType)
            {
            case 0:
                DrawLine(ImVec2(g_Screen.x - Width - 1.f, pTop.y), ImVec2(g_Screen.x + Width + 1.f, pTop.y), color, 1.f); // ��
                DrawLine(ImVec2(g_Screen.x - Width, pTop.y), ImVec2(g_Screen.x - Width, pBottom.y), color, 1.f); // ��
                DrawLine(ImVec2(g_Screen.x + Width, pTop.y), ImVec2(g_Screen.x + Width, pBottom.y), color, 1.f); // �E
                DrawLine(ImVec2(g_Screen.x - Width - 1.f, pBottom.y), ImVec2(g_Screen.x + Width + 1.f, pBottom.y), color, 1.f); // ��
                break;
            case 1:
                // OutLine
                DrawLine(ImVec2(g_Screen.x - Width - 1.f, pTop.y - 1.f), ImVec2(g_Screen.x + Width + 1.f, pTop.y - 1.f), ImColor(0.f, 0.f, 0.f, 1.f), 1.f); // ��
                DrawLine(ImVec2(g_Screen.x - Width - 1.f, pTop.y), ImVec2(g_Screen.x - Width - 1.f, pBottom.y), ImColor(0.f, 0.f, 0.f, 1.f), 1.f); // ��
                DrawLine(ImVec2(g_Screen.x + Width + 1.f, pTop.y), ImVec2(g_Screen.x + Width + 1.f, pBottom.y), ImColor(0.f, 0.f, 0.f, 1.f), 1.f); // �E
                DrawLine(ImVec2(g_Screen.x - Width - 1.f, pBottom.y + 1.f), ImVec2(g_Screen.x + Width + 1.f, pBottom.y + 1.f), ImColor(0.f, 0.f, 0.f, 1.f), 1.f); // ��

                // �� | ��-�E
                DrawLine(ImVec2(g_Screen.x - Width, pTop.y), ImVec2(g_Screen.x - (Width / 2.5f), pTop.y), color, 1.f);
                DrawLine(ImVec2(g_Screen.x + Width, pTop.y), ImVec2(g_Screen.x + (Width / 2.5f), pTop.y), color, 1.f);
                // �� | ��-��
                DrawLine(ImVec2(g_Screen.x - Width, pTop.y), ImVec2(g_Screen.x - Width, pTop.y + (Width / 1.5f)), color, 1.f);
                DrawLine(ImVec2(g_Screen.x - Width, pBottom.y), ImVec2(g_Screen.x - Width, pBottom.y - (Width / 1.5f)), color, 1.f);
                // �E | ��-��
                DrawLine(ImVec2(g_Screen.x + Width, pTop.y), ImVec2(g_Screen.x + Width, pTop.y + (Width / 1.5f)), color, 1.f);
                DrawLine(ImVec2(g_Screen.x + Width, pBottom.y), ImVec2(g_Screen.x + Width, pBottom.y - (Width / 1.5f)), color, 1.f);
                // �� | ��-�E
                DrawLine(ImVec2(g_Screen.x - Width, pBottom.y), ImVec2(g_Screen.x - (Width / 2.5f), pBottom.y), color, 1.f);
                DrawLine(ImVec2(g_Screen.x + Width, pBottom.y), ImVec2(g_Screen.x + (Width / 2.5f), pBottom.y), color, 1.f);
                break;
            default:
                break;
            }

            // Filled
            if (g.ESP_BoxFilled)
                ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(g_Screen.x - Width, pTop.y), ImVec2(g_Screen.x + Width, pBottom.y), ImColor(0.f, 0.f, 0.f, 0.35f), NULL);
        }

        // Line
        if (g.ESP_Line)
            DrawLine(ImVec2(g.GameRect.right / 2, g.GameRect.bottom), ImVec2(g_Screen.x, pBottom.y), color, 1);

        // Bone ESP
        if (g.ESP_Skeleton)
        {
            int aSkeleton[][2] =
            {
            { 104, 142 },{ 142, 9 },{ 9, 11 },{ 11, 15 },
            { 142,109},{109,111 },{111, 115},{ 142, 5 },
            { 5,  188},{ 5, 197},{ 188, 184},{ 197, 198},
            };

            for (int j = 0; j < 12; ++j)
            {
                Vector2 Out1{}, Out2{};
                Vector3 Bone1 = pEntity->GetBone(aSkeleton[j][0]);
                Vector3 Bone2 = pEntity->GetBone(aSkeleton[j][1]);

                if (Bone1 == Vector3(0.f, 0.f, 0.f) || Bone2 == Vector3(0.f, 0.f, 0.f))
                    break;

                if (WorldToScreen(Bone1, Out1) && WorldToScreen(Bone2, Out2))
                    DrawLine(ImVec2(Out1.x, Out1.y), ImVec2(Out2.x, Out2.y), g.ESP_SkeletonColor == 0 ? color : ESP_Skeleton, 1);
            }
        }

        // Health Bar
        if (g.ESP_HealthBar)
            HealthBar((g_Screen.x - Width) - 4.f, pBottom.y, 2.f, -Height - 1, pEntity->hComponent.m_Health, pEntity->hComponent.m_MaxHealth);

        // Distance
        if (g.ESP_Distance)
        {
            std::string DistString = std::to_string((int)distance) + "m";
            StrokeText(DistString.c_str(), ImVec2(g_Screen.x, g_Screen.y + 2.f), ImColor(1.f, 1.f, 1.f, 1.f), 14.f, true);
        }

        // Name
        if (g.ESP_Name)
            StrokeText(pEntity->GetName(), ImVec2(g_Screen.x, pTop.y - 14.f), ImColor(1.f, 1.f, 1.f, 1.f), 14.f, true);

        // AimBot
        if (g.AimBot)
        {
            if (!IsKeyDown(g.AimKey0) && !IsKeyDown(g.AimKey1))
                continue;
            else if (GetForegroundWindow() != GameWindow)
                continue;
            else if (!g.AimAtTeam && pEntity->Team == pLocal->Team)
                continue;
            else if (g.Aim_MaxDistance < distance)
                continue;
            else if (g.VisCheck && !pEntity->IsVisible())
                continue;
            else if (pLocal->InVehicle())
                continue;

            bool AimReady = false;
            std::vector<int> BoneList = { 104, 142, 9, 11, 15, 109, 111, 115, 5, 188, 184, 197, 198 };

            for (auto Bone : BoneList)
            {
                Vector2 pTarget = Vector2(0.f, 0.f);
                Vector3 pBonePos = pEntity->GetBone(Bone);

                if (pBonePos == Vector3(0.f, 0.f, 0.f))
                    continue;

                if (!WorldToScreen(pBonePos, pTarget))
                    continue;

                // Fov check
                FOV = abs((ScreenMiddle - pTarget).Length());

                if (FOV < g.AimFov)
                {
                    AimReady = true;
                    break;
                }
            }

            BoneList.clear();

            if (AimReady)
            {
                Vector2 TargetScreen = Vector2(0.f, 0.f);
                if (!WorldToScreen(pEntity->GetBone(g.AimTargetBone == 0 ? 104 : 6), TargetScreen))
                    continue;

                switch (g.AimType)
                {
                case 0:
                    if (MinFov > FOV)
                    {
                        MinFov = FOV;
                        TargetPosition = TargetScreen;
                    }
                    break;
                case 1:
                    if (MinDistance > distance)
                    {
                        MinDistance = distance;
                        TargetPosition = TargetScreen;
                    }
                    break;
                default:
                    break;
                }
            }
        }
    }

    if (TargetPosition != Vector2(0.f, 0.f))
        this->AimBot(TargetPosition);
}

void SliderEx(const char* Title, const char* SliderText, const char* Format, float& Value, float Min, float Max)
{
    ImGui::Text(Title);
    ImGui::Spacing(); ImGui::SameLine();
    ImGui::SliderFloat(SliderText, &Value, Min, Max); ImGui::SameLine();
    ImGui::Text(Format, Value);
}

void SliderEx(const char* Title, const char* SliderText, const char* Format, int& Value, int Min, int Max)
{
    ImGui::Text(Title);
    ImGui::Spacing(); ImGui::SameLine();
    ImGui::SliderInt(SliderText, &Value, Min, Max); ImGui::SameLine();
    ImGui::Text(Format, Value);
}