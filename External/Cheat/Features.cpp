#include "Cheat.h"

// 104 -> Head // 6 -> Chest(spine)
int AimBone = 104;

// mouse_event based aimbot
bool Cheat::AimBot(Vector2& TargetPosition)
{
    /*----| KeyChecks |--------------------------------------------------------------------------------*/
    switch (g.AimKeyType)
    {
    case 0: // and
        if (g.AimKey1 == NULL && IsKeyDown(g.AimKey0))
            break;
        else if (!IsKeyDown(g.AimKey0) || !IsKeyDown(g.AimKey1))
            return false;
        else if (!IsKeyDown(g.AimKey0))
            return false;
        break;
    case 1: // or
        if (g.AimKey1 == NULL && IsKeyDown(g.AimKey0))
            break;
        else if (IsKeyDown(g.AimKey0) || IsKeyDown(g.AimKey1))
            break;

        break;
    }
    /*--------------------------------------------------------------------------------------------------*/

    Vector2 ScreenMiddle = { (float)g.GameRect.right / 2.f, (float)g.GameRect.bottom / 2.f };

    // AimBot実行
    int cX = (ScreenMiddle.x - TargetPosition.x) / g.Smooth;
    int cY = (ScreenMiddle.y - TargetPosition.y) / g.Smooth;

    mouse_event(MOUSEEVENTF_MOVE, -cX, -cY, 0, 0);
}

void Cheat::Misc()
{
    while (g.Run)
    {
        uint64_t SyncBFSetting = m.Read<uint64_t>(offset::SyncBFSetting);
        uint64_t Weapon = m.Read<uint64_t>(offset::ClientWeapons);

        if (!Weapon || !SyncBFSetting)
        {
            Sleep(10);
            continue;
        }

        uint64_t weaponPtr = m.Read<uint64_t>(Weapon + 0x128);
        uint64_t GunSwayData = m.Read<uint64_t>(weaponPtr + 0x30); // Sway
        uint64_t FiringFunctionData = m.Read<uint64_t>(weaponPtr + 0x10);
        CurrentBPS = m.Read<int>(FiringFunctionData + 0xD8);

        // Recoil / Spread
        if (g.SwayModify && GunSwayData != 0 && m.Read<float>(GunSwayData + 0x430) != g.ModVal)
        {
            m.Write<float>(GunSwayData + 0x430, g.ModVal);
            m.Write<float>(GunSwayData + 0x438, g.ModVal);
            m.Write<float>(GunSwayData + 0x434, g.ModVal);
            m.Write<float>(GunSwayData + 0x43C, g.ModVal);
        }
        else if (!g.SwayModify && GunSwayData != 0 && m.Read<float>(GunSwayData + 0x430) != 1.f)
        {
            m.Write<float>(GunSwayData + 0x430, 1.f);
            m.Write<float>(GunSwayData + 0x438, 1.f);
            m.Write<float>(GunSwayData + 0x434, 1.f);
            m.Write<float>(GunSwayData + 0x43C, 1.f);
        }

        // UnlockAll
        if (g.UnlockAll && SyncBFSetting != 0 && m.Read<bool>(SyncBFSetting + 0x54) == false)
            m.Write<bool>(SyncBFSetting + 0x54, true);

        // Set DefaultFOV
        if (this->DefaultFov == 0.f && this->DefaultViewModelFov == 0.f)
        {
            GetFOV(DefaultFov, DefaultViewModelFov);
            g.NormalFOV = DefaultFov;
            g.ViewModelFOV = DefaultViewModelFov;
        }
        
        // Fov Changer
        if (g.FovChanger && g.ForceUserFov)
        {
            // Normal Fov
            uintptr_t FovBase = m.Read<uintptr_t>(0x142670D80);
            uintptr_t ViewModelFovBase = m.Read<uintptr_t>(0x1423819B0);

            if (!FovBase || !ViewModelFovBase)
                continue;

            if (m.Read<float>(FovBase + 0x90) != g.NormalFOV)
                m.Write<float>(FovBase + 0x90, g.NormalFOV);

            if (m.Read<float>(FovBase + 0x90) != g.ViewModelFOV)
                m.Write<float>(m.Read<uint64_t>(ViewModelFovBase + 0x378) + 0x590, g.ViewModelFOV);
        }

        Sleep(1);
    }
}

void Cheat::GetFOV(float& Normal, float& ViewModel)
{
    Normal = m.Read<float>(m.Read<uintptr_t>(0x142670D80) + 0x90);
    ViewModel = m.Read<float>(m.Read<uint64_t>(m.Read<uintptr_t>(0x1423819B0) + 0x378) + 0x590);
}

void Cheat::SetFOV(const float &v1, const float &v2)
{
    // Normal Fov
    uint64_t FovBase = m.Read<uint64_t>(0x142670D80);
    m.Write<float>(FovBase + 0x90, v1);

    // ViewModel Fov
    uint64_t ViewModelFovBase = m.Read<uint64_t>(0x1423819B0);
    m.Write<float>(m.Read<uint64_t>(ViewModelFovBase + 0x378) + 0x590, v2);
}

void Cheat::SetDefaultFOV()
{
    // Normal Fov
    uint64_t FovBase = m.Read<uint64_t>(0x142670D80);
    m.Write<float>(FovBase + 0x90, this->DefaultFov);

    // ViewModel Fov
    uint64_t ViewModelFovBase = m.Read<uint64_t>(0x1423819B0);
    m.Write<float>(m.Read<uint64_t>(ViewModelFovBase + 0x378) + 0x590, this->DefaultViewModelFov);
}

bool Cheat::SetBPS(int value)
{
    if (!g.DamageHack)
        return false;

    uint64_t Weapon = m.Read<uint64_t>(offset::ClientWeapons);

    if (!Weapon)
        return false;

    uint64_t weaponPtr = m.Read<uint64_t>(Weapon + 0x128);
    uint64_t FiringFunctionData = m.Read<uint64_t>(weaponPtr + 0x10);

    if (!FiringFunctionData)
        return false;

    m.Write<int>(FiringFunctionData + 0xD8, value);

    return true;
}

void Cheat::KeyBinder(int &target_key)
{
    // チェック用変数
    bool flag = false;

    // KeyBinder
    while (true)
    {
        for (int i = 0; i < 0x87; i++)
        {
            if (i == VK_LWIN || i == VK_RWIN)
                continue;

            if (IsKeyDown(i))
            {
                if (i == VK_ESCAPE)
                {
                    target_key = NULL;
                    flag = true;
                }  
                else
                {
                    target_key = i;
                    flag = true;
                }

                break;
            }
        }

        if (flag)
            break;
    }

    // Check
    if (g.AimKey0 == g.AimKey1)
        g.AimKey1 = 0;

    g.KeyBinding = false;
    g.BindingID = 0;
}

void Cheat::RainbowGenerator()
{
    while (g.Run)
    {
        static float rainbow;
        rainbow += 0.01f;
        if (rainbow > 1.f)
            rainbow = 0.f;

        constexpr float factor = 6.0f;
        int i = static_cast<int>(rainbow * factor);
        float f = rainbow * factor - i;
        float q = 1 - f;
        float t = 1 - (1 - f);

        Rainbow.Value.x = (i % 6 == 0) ? 1.f : (i % 6 == 1) ? q : (i % 6 == 2) ? 0 : (i % 6 == 3) ? 0 : (i % 6 == 4) ? t : 1.f;
        Rainbow.Value.y = (i % 6 == 0) ? t : (i % 6 == 1) ? 1.f : (i % 6 == 2) ? 1.f : (i % 6 == 3) ? q : (i % 6 == 4) ? 0 : 0;
        Rainbow.Value.z = (i % 6 == 0) ? 0 : (i % 6 == 1) ? 0 : (i % 6 == 2) ? t : (i % 6 == 3) ? 1.f : (i % 6 == 4) ? 1.f : q;

        std::this_thread::sleep_for(std::chrono::milliseconds(g.RainbowRate));
    }
}