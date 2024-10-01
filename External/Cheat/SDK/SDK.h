#pragma once
#include "..\..\Utils\Memory\Memory.h"
#include <SimpleMath.h>
using namespace DirectX::SimpleMath;

namespace offset
{
    constexpr auto ClientgameContext = 0x142670D80;
    constexpr auto SyncBFSetting = 0x1423717C0;
    constexpr auto GameRenderer = 0x142672378;
    constexpr auto DxRenderer = 0x142738080;
    constexpr auto ClientWeapons = 0x1423B2EC8;

    constexpr auto PlayerManager = 0x60;
    constexpr auto LocalPlayer = 0x540;
    constexpr auto ClientPlayer = 0x548;
    constexpr auto ClientSoldier = 0x14D0;
    constexpr auto ClientVehicle = 0x14C0;

    constexpr auto PlayerTeam = 0x13CC;
    constexpr auto PlayerName = 0x1836;
    constexpr auto Occlude = 0x5B1;
    constexpr auto Spectator = 0x13C9;
}

struct AxisAlignedBox
{
    Vector4 Min;
    Vector4 Max;
};

class HealthComponent
{
public:
    char _0x0000[32];
    float m_Health; //0x0020 
    float m_MaxHealth; //0x0024 
    char _0x0028[16];
    float m_VehicleHealth; //0x0038 
    float m_MaxVehicleHealth; //0x0038 
};

class CPlayer
{
public:
    // ptr
    uintptr_t ClientPlayer;
    uintptr_t ClientSoldier;
    uintptr_t ClientVehicle;
    uintptr_t ClientVehicleData;
    uintptr_t pQuat;
    
    // Data
    int Team;
    HealthComponent hComponent;
    float vMaxHealth;
    Vector3 Position;
    std::string Name;
    bool Occlude;
    int Pose;
    
    Matrix VehicleTranfsorm;
    AxisAlignedBox SoldierAABB;
    AxisAlignedBox VehicleAABB;

    void GetPlayer(uintptr_t& player) noexcept
    {
        ClientPlayer = player;
    }
    bool IsValid() noexcept
    {
        return ClientPlayer != NULL;
    }
    bool InVehicle() noexcept
    {
        return ClientVehicle != NULL;
    }
    bool IsSpectator()
    {
        return m.Read<bool>(ClientPlayer + 0x13C9);
    }
    bool IsVisible() noexcept
    {
        return !Occlude;
    }
    bool IsDead() noexcept
    {
        return hComponent.m_Health <= 0.f && Position == Vector3(0.f, 0.f, 0.f);
    }
    std::string GetName()
    {
        char pName[64];
        m.ReadString(ClientPlayer + offset::PlayerName, &pName, sizeof(pName));

        return pName;
    }
    uint64_t GetQuat()
    {
        return m.Read<uint64_t>(m.Read<uint64_t>(ClientSoldier + 0x580) + 0xB0);
    }
    void Update()
    {
        ClientSoldier = m.Read<uint64_t>(ClientPlayer + offset::ClientSoldier);
        ClientVehicle = m.Read<uint64_t>(ClientPlayer + offset::ClientVehicle);

        if (InVehicle())
        {
            VehicleAABB = m.Read<AxisAlignedBox>(ClientVehicle + 0x250);
            ClientVehicleData = m.Read<uintptr_t>(ClientSoldier + 0x30);
            vMaxHealth = m.Read<float>(ClientVehicleData + 0x148);  // MaxHealth

            // Position
            uintptr_t DynamicPhysicsEntity = m.Read<uintptr_t>(ClientVehicle + 0x238);

            if (DynamicPhysicsEntity)
            {
                uintptr_t pPhysicsEntity = m.Read<uintptr_t>(DynamicPhysicsEntity + 0xA0);
                VehicleTranfsorm = m.Read<Matrix>(pPhysicsEntity);

                Position = Vector3(VehicleTranfsorm._41, VehicleTranfsorm._42, VehicleTranfsorm._43);
            }
        }
        else
        {
            Position = m.Read<Vector3>(m.Read<uint64_t>(ClientSoldier + 0x490) + 0x30);
        }

        // Team
        Team = m.Read<int>(ClientPlayer + offset::PlayerTeam);

        // Health
        uint64_t HealthComponentPtr = m.Read<uint64_t>(ClientSoldier + 0x140);
        hComponent = m.Read<HealthComponent>(HealthComponentPtr);

        // Visible
        Occlude = m.Read<bool>(ClientSoldier + 0x5B1);
        Pose = m.Read<int>(ClientSoldier + 0x4F0);
        pQuat = GetQuat();
        Name = GetName();
    }
    Vector3 GetBone(int bone_id)
    {
        return m.Read<Vector3>(this->pQuat + bone_id * 0x20);
    }
    AxisAlignedBox GetAABB()
    {
        AxisAlignedBox aabb = AxisAlignedBox();

        switch (Pose)
        {
        case 0:
            aabb.Min = Vector4(-0.350000f, 0.000000f, -0.350000f, 0);
            aabb.Max = Vector4(0.350000f, 1.700000f, 0.350000f, 0);
            break;
        case 1:
            aabb.Min = Vector4(-0.350000f, 0.000000f, -0.350000f, 0);
            aabb.Max = Vector4(0.350000f, 1.150000f, 0.350000f, 0);
            break;
        case 2:
            aabb.Min = Vector4(-0.350000f, 0.000000f, -0.350000f, 0);
            aabb.Max = Vector4(0.350000f, 0.400000f, 0.350000f, 0);
            break;
        default:
            break;
        }

        return aabb;
    }
};

extern bool UpdateW2SData();
extern float GetDistance(Vector3 value1, Vector3 value2);
extern bool WorldToScreen(const Vector3& WorldPos, Vector2& ScreenPos);