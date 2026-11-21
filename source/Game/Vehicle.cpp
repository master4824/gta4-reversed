#include "StdInc.h"
#include "Game/Vehicle.h"
#include "Game/ModelInfo.h"
#include "Game/Ped.h"
#include "Game/Timer.h"
#include "Core/Hook.h"
#include "Core/Log.h"
#include <cassert>
#include <cmath>
#include <cstring>

struct CVehicleSubObj5160
{
    bool __thiscall IsInNegativeRange();
};

struct CVehicleSubObj5180
{
    CVehicleSubObj5180* __thiscall Ctor();
};

struct CVehicleSubObjE24
{
    CVehicleSubObjE24* __thiscall Ctor();
};

struct CVehicleStateSnapshot
{
    uint8 m_data[0x70] = {};

    void __thiscall Save(CVehicle* pVeh);
    void __thiscall Restore(CVehicle* pVeh);
};

struct CVehicleLink
{
    void*     m_pAttached; // +0x00
    void*     m_pPending;  // +0x04
    CVehicle* m_pOwner;    // +0x08
    int32     m_nBoneId;   // +0x0C
    uint8     m_bFlag;     // +0x10
    uint8     pad11;       // +0x11
    int16     m_nTimer;    // +0x12

    CVehicleLink* __thiscall Ctor();
    void  __thiscall Dtor();
    void  __thiscall SetBoneId(int32 boneId);
    void  __thiscall SetAttached(CEntity* pEntity);
    bool  __thiscall IsAttached(const CEntity* pEntity);
    void  __thiscall SetPending(CEntity* pEntity);
    bool  __thiscall HasAnyEntity();
    bool  __thiscall IsValidBoneId();
    int32 __thiscall GetBoneIndex();
    void  __thiscall UpdateAudio(bool bUsePending);
    void  __thiscall Set(void* pEntity, bool bSet, bool bDoUpdate);
    bool  __thiscall Attach(void* pEntity);
    bool  __thiscall Detach(void* pEntity);
    void  __thiscall Process();
};

void CVehicle::InjectHooks()
{
    Core::Hook::Install(0x9C7C40, &CVehicle::Ctor);
    Core::Hook::Install(0x9C6D20, &CVehicle::GetPassenger);
    Core::Hook::Install(0x9C6D70, &CVehicle::SeatToPassengerIndex);
    Core::Hook::Install(0x9C6DB0, &CVehicle::PassengerIndexToSeat);
    Core::Hook::Install(0x9C52C0, &CVehicle::ResetSubObjAndAudio);
    Core::Hook::Install(0x9C5360, &CVehicle::IsFlagF6D10Set);
    Core::Hook::Install(0x9C5370, &CVehicle::TrySetFlagF6D10);
    Core::Hook::Install(0x9C53C0, &CVehicle::GetTimer12F4Progress);
    Core::Hook::Install(0x9C5430, &CVehicle::IsUsingPretendOccupants);
    Core::Hook::Install(0x9C6DF0, &CVehicle::SeatToDoorIndex);
    Core::Hook::Install(0x9C6E40, &CVehicle::GetPedSeat);
    Core::Hook::Install(0x9C6E90, &CVehicle::GetSeatForTargetId);
    Core::Hook::Install(0x9C6EF0, &CVehicle::GetAdjustedSeatIndex);
    Core::Hook::Install(0x9C6F20, &CVehicle::GetSeatNameById);
    Core::Hook::Install(0x9C6F90, &CVehicle::UpdateVehUnkF84);
    Core::Hook::Install(0x9C6FD0, &CVehicle::IsPlayerInVehicle);
    Core::Hook::Install(0x9C5440, &CVehicle::HasDummyOccupants);
    Core::Hook::Install(0x9C5480, &CVehicle::HasAnyOccupants);
    Core::Hook::Install(0x9C54C0, &CVehicle::HasAnyUninjuredOccupants);
    Core::Hook::Install(0x9C5510, &CVehicle::FindSubObjFD0);
    Core::Hook::Install(0x9C5570, &CVehicle::CountActiveSubObjFD0);
    Core::Hook::Install(0x9C55E0, &CVehicle::FindSubObjFD8);
    Core::Hook::Install(0x9C5630, &CVehicle::GetModelBounds);
    Core::Hook::Install(0x9C5650, &CVehicle::UpdateTimer130E);
    Core::Hook::Install(0x9C56E0, &CVehicle::IsObjectFlagSet);
    Core::Hook::Install(0x9C5760, &CVehicle::RemovePassenger);
    Core::Hook::Install(0x9C6AF0, &CVehicle::GetModelSeatEntry);
    Core::Hook::Install(0x9C6B20, &CVehicle::GetModelSeatCount);
    Core::Hook::Install(0x9C6B40, &CVehicle::HasAnyActiveLink);
    Core::Hook::Install(0x9C6B70, &CVehicle::FindLinkIndexByKey);
    Core::Hook::Install(0x9C6BA0, &CVehicle::GetLink);
    Core::Hook::Install(0x9C6BC0, &CVehicle::AllocateLink);
    Core::Hook::Install(0x9C6C00, &CVehicle::InitVehicleConfig);
    Core::Hook::Install(0x9C6CF0, &CVehicle::GetPassengerCount);
    Core::Hook::Install(0x9C7010, &CVehicle::SetOccupantsPropFlag100);
    Core::Hook::Install(0x9C7170, &CVehicle::AddVehUnk10A0);
    Core::Hook::Install(0x9C71C0, &CVehicle::AddVehUnk10B0);
    Core::Hook::Install(0x9C7210, &CVehicle::AddCrossVehUnk10B0);
    Core::Hook::Install(0x9C72B0, &CVehicle::IsAudioSubTypeF);
    Core::Hook::Install(0x9C72D0, &CVehicle::ProcessMovingAudio1348);
    Core::Hook::Install(0x9C7380, &CVehicle::ProcessOccupantDamageF67);
    Core::Hook::Install(0x9C7560, &CVehicle::GetDrawHandlerUnkD8);
    Core::Hook::Install(0x9C7580, &CVehicle::SetDrawHandlerUnkD8);
    Core::Hook::Install(0x9C75A0, &CVehicleStateSnapshot::Save);
    Core::Hook::Install(0x9C7670, &CVehicleStateSnapshot::Restore);
    Core::Hook::Install(0x9C7710, &CVehicle::GetBoneLocalMatrix);
    Core::Hook::Install(0x9C7960, &CVehicleLink::GetBoneIndex);
    Core::Hook::Install(0x9C7970, &CVehicleLink::UpdateAudio);
    Core::Hook::Install(0x9C79F0, &CVehicleLink::Set);
    Core::Hook::Install(0x9C7A40, &CVehicleLink::Attach);
    Core::Hook::Install(0x9C7AA0, &CVehicleLink::Detach);
    Core::Hook::Install(0x9C7B00, &CVehicleLink::Process);
    Core::Hook::Install(0x9C77F0, &CVehicleSubObjE24::Ctor);
    Core::Hook::Install(0x9C7BF0, &CVehicle::InitSystem);
    Core::Hook::Install(0x9C80B0, &CVehicle::ProcessControlStub);
    Core::Hook::Install(0x9C80C0, &CVehicle::GetDefaultMaxPassengers);
    Core::Hook::Install(0x9C80D0, &CVehicle::PreRender);
    Core::Hook::Install(0x9C83A0, &CVehicle::CreateModelInstance);
    Core::Hook::Install(0x9C8700, &CVehicle::GetBoneWorldPos);
    Core::Hook::Install(0x9C8900, &CVehicle::GetBoneMatrix);
    Core::Hook::Install(0x9C8A70, &CVehicle::UpdateDamageBoneTable);
    Core::Hook::Install(0x9C8C70, &CVehicle::CanBeConvertedToDummy);
    Core::Hook::Install(0x9C8D80, &CVehicle::CanUsePretendOccupants);
    Core::Hook::Install(0x9C8DC0, &CVehicle::TrySpawnInWorld);
    Core::Hook::Install(0x9C8F30, &CVehicle::IsEnterable);
    Core::Hook::Install(0x9C9020, &CVehicle::TryEnablePretendOccupants);
    Core::Hook::Install(0x9C91A0, &CVehicle::ClearOccupantsAndSetFlag400);
    Core::Hook::Install(0x9C9210, &CVehicle::TryDisablePretendOccupants);
    Core::Hook::Install(0x9C93B0, &CVehicle::CollectEntityAndOccupants);
    Core::Hook::Install(0x9C9BA0, &CVehicle::CheckVehicleUprightAndState);
    Core::Hook::Install(0x9C9E60, &CVehicle::UpdateWheelSteerOrLean);
    Core::Hook::Install(0x9CA080, &CVehicle::AddPassenger);
    Core::Hook::Install(0x9CA250, &CVehicle::AddPassengerAtSeat);
    Core::Hook::Install(0x9CA440, &CVehicle::ProcessBurningOccupants);
    Core::Hook::Install(0x9CA600, &CVehicle::IsUpsideDown);
    Core::Hook::Install(0x9CA620, &CVehicle::IsOnSide);
    Core::Hook::Install(0x9CA650, &CVehicle::CanPedEnterWithLockState);
    Core::Hook::Install(0x9CA710, &CVehicle::IsVehicleStationaryAndLevel);
    Core::Hook::Install(0x9CA8E0, &CVehicle::ShouldPedBailFromSpeed);
    Core::Hook::Install(0x9CA9B0, &CVehicle::TryStartTimerF98);
    Core::Hook::Install(0x9CA9E0, &CVehicle::IsSpecialModelIndex);
    Core::Hook::Install(0x9CAA30, &CVehicle::ResetHealthAndSubObjFD0);
    Core::Hook::Install(0x9CC990, &CVehicle::PlayPedEnterAnim);
    Core::Hook::Install(0x9D4A50, &CVehicle::InitPretendOccupants);
    Core::Hook::Install(0x9D5ED0, &CVehicle::ScalarDeletingDtor);
    Core::Hook::Install(0x9D5F00, &CVehicle::InitFragAndPhysics);
    Core::Hook::Install(0x9D5FC0, &CVehicle::CleanupPhysicsAndAudio);
    Core::Hook::Install(0x9D6170, &CVehicle::SetHeading);
    Core::Hook::Install(0x9C5120, &CVehicle::ArePedsAwareOrHostile);
    Core::Hook::Install(0x9C5160, &CVehicleSubObj5160::IsInNegativeRange);
    Core::Hook::Install(0x9C5180, &CVehicleSubObj5180::Ctor);
    Core::Hook::Install(0x9C51B0, &CVehicleLink::Ctor);
    Core::Hook::Install(0x9C51D0, &CVehicleLink::Dtor);
    Core::Hook::Install(0x9C5200, &CVehicleLink::SetBoneId);
    Core::Hook::Install(0x9C5210, &CVehicleLink::SetAttached);
    Core::Hook::Install(0x9C5240, &CVehicleLink::IsAttached);
    Core::Hook::Install(0x9C5250, &CVehicleLink::SetPending);
    Core::Hook::Install(0x9C5280, &CVehicleLink::HasAnyEntity);
    Core::Hook::Install(0x9C52A0, &CVehicleLink::IsValidBoneId);
    Core::Hook::Install(0x9C52E0, &CVehicle::ResetDrawHandler);
    Core::Hook::Install(0x9C5300, &CVehicle::GetVehicleClassFromFlags);
    Core::Hook::Install(0x9C5B00, &CVehicle::RemoveDriver);
    Core::Hook::Install(0x9C5B50, &CVehicle::TryAddRandomDriver);
    Core::Hook::Install(0x9C5BC0, &CVehicle::TryAddRandomPassenger);
    Core::Hook::Install(0x9C5D30, &CVehicle::IsPedPassenger);
    Core::Hook::Install(0x9C5D60, &CVehicle::IsPedDriver);
    Core::Hook::Install(0x9C5D80, &CVehicle::IsDoorLockStateUnlocked);
    Core::Hook::Install(0x9C5DA0, &CVehicle::ProcessAlarm);
    Core::Hook::Install(0x9C5E20, &CVehicle::IsPointInBoundingBox);
    Core::Hook::Install(0x9C5F30, &CVehicle::GetVehicleAnimGroupIndex);
    Core::Hook::Install(0x9C5F50, &CVehicle::IsSpecialVehicleModel);
    Core::Hook::Install(0x9C5FD0, &CVehicle::IsConvertibleRoofClosedOrValid);
    Core::Hook::Install(0x9C57E0, &CVehicle::SetDriver);
    Core::Hook::Install(0x9C6040, &CVehicle::UpdateUpsideDownAlarmOrFireFlag);
    Core::Hook::Install(0x9C60C0, &CVehicle::IsOnFireFlagActive);
    Core::Hook::Install(0x9C6100, &CVehicle::GetExcessSpeedOrInvalid);
    Core::Hook::Install(0x9C6140, &CVehicle::GetRandomPassenger);
    Core::Hook::Install(0x9C61A0, &CVehicle::HasAnyActiveOccupant219);
    Core::Hook::Install(0x9C61E0, &CVehicle::HasAliveOccupants);
    Core::Hook::Install(0x9C6230, &CVehicle::IsPedOccupant);
    Core::Hook::Install(0x9C6270, &CVehicle::IsInActiveRange);
    Core::Hook::Install(0x9C6380, &CVehicle::CanBeEnteredByPed);
    Core::Hook::Install(0x9C63B0, &CVehicle::DecrementUnk133C);
    Core::Hook::Install(0x9C63D0, &CVehicle::ShouldSkipLodOrUpdate);
    Core::Hook::Install(0x9C6500, &CVehicle::ResetBoneChild);
    Core::Hook::Install(0x9C65B0, &CVehicle::SetBoneHidden);
    Core::Hook::Install(0x9C6670, &CVehicle::IsBoneVisible);
    Core::Hook::Install(0x9C66B0, &CVehicle::SetCreatedBy);
    Core::Hook::Install(0x9C6710, &CVehicle::ClearUnkFlagsF64_1344);
    Core::Hook::Install(0x9C6720, &CVehicle::GetBurnoutOrEngineHealthFactor);
    Core::Hook::Install(0x9C6870, &CVehicle::HasAnyWheelUnk160Not1000);
    Core::Hook::Install(0x9C69B0, &CVehicle::HasAnyWheelUnk15CNot1000);
    Core::Hook::Install(0x9C6B10, &CVehicle::GetModelInfoEntryIndex);
    Core::Hook::Install(0x9C9450, &CVehicle::ResolveContactPenetration);
    Core::Hook::Install(0x9CAB30, &CVehicle::ProcessFlyingPhysics);
    Core::Hook::Install(0x9D3B40, &CVehicle::FindOrAllocateLink);
    Core::Hook::Install(0x9D3B90, &CVehicle::ProcessLinks);
    Core::Hook::Install(0x9D3BC0, &CVehicle::GetLinkForSeat);
    Core::Hook::Install(0x9D3BF0, &CVehicle::GetLinkForSeatSlot);
    Core::Hook::Install(0x9D3C80, &CVehicle::GetUnkSeatOffset);
    Core::Hook::Install(0x9D3D60, &CVehicle::SaveStateSnapshot);
    Core::Hook::Install(0x9D3DA0, &CVehicle::LoadStateSnapshot);
    Core::Hook::Install(0x9D3DE0, &CVehicle::IsBoneNonZero);
    Core::Hook::Install(0x9D3E70, &CVehicle::ResetBoneChildIfNonZero);
    Core::Hook::Install(0x9D43A0, &CVehicle::InitVehiclePool);
    Core::Hook::Install(0x9D4AC0, &CVehicle::ApplyHiddenBoneMask);
    Core::Hook::Install(0x9DFE70, &CVehicle::UpdateSubObj80Position);
    Core::Hook::Install(0x9D2410, &CVehicle::ProcessDriverDropWeapon);
    Core::Hook::Install(0x9D25D0, &CVehicle::ProcessCollisions);
    Core::Hook::Install(0x9D2A00, &CVehicle::InitBrokenBoneMask);
    Core::Hook::Install(0x9D31A0, &CVehicle::UpdateDoorOpenRatio);
    Core::Hook::Install(0x9D3330, &CVehicle::UpdateDoorCloseRatio);
    Core::Hook::Install(0x9D3480, &CVehicle::UpdateUnk10D8_10C8_10CC);
    Core::Hook::Install(0x9D3600, &CVehicle::SetEngineOrBurnoutState);
    Core::Hook::Install(0x9D36A0, &CVehicle::ProcessEngineOrBurnout);
    Core::Hook::Install(0x9D3A10, &CVehicle::ProcessInteriorLightOrUnk1114);
    Core::Hook::Install(0x9D3E90, &CVehicle::UpdateDirtLevel);
    Core::Hook::Install(0x9D43E0, &CVehicle::Dtor);
    Core::Hook::Install(0x9D45E0, &CVehicle::SetModelIndex);
    Core::Hook::Install(0x9D0D60, &CVehicle::ProcessLightsAndEffects);
    Core::Hook::Install(0x9D3F60, &CVehicle::ApplyModelDoorRotation);
    Core::Hook::Install(0x9D4120, &CVehicle::IsClearAbove);
    Core::Hook::Install(0x9D41F0, &CVehicle::UpdateInteriorModelStream);
    Core::Hook::Install(0x9D49C0, &CVehicle::GetBoneEntryUnkC299A0);
    Core::Hook::Install(0x9D4A10, &CVehicle::GetBoneEntryUnk9C8900);
    Core::Hook::Install(0x9D4AA0, &CVehicle::RegisterInUnk18A82AC);
    Core::Hook::Install(0x9D4BD0, &CVehicle::ComputeSeatEntryTransform);
    Core::Hook::Install(0x9D5DA0, &CVehicle::DetachFragPart);
    Core::Hook::Install(0x9D6090, &CVehicle::UpdateNetworkOrPhysicsActive);
    Core::Hook::Install(0x9D62C0, &CVehicle::ProcessControl);

    Core::Log::Info("CVehicle hooks installed (170 functions)");
}

CVehicle* __thiscall CVehicle::Ctor(uint8 createdBy)
{
    CPhysical::Ctor();
    m_vftable = reinterpret_cast<void**>(Core::Rebase(0xD82164));

    Core::Hook::CallMethod<void>(0xAEBB10, m_vehSubObj260);
    Core::Hook::CallMethod<void>(0xCBB490, m_vehAudioEntity);

    Core::Hook::CallMethod<void>(0xCF0E80, m_audPlaceableTracker);
    *reinterpret_cast<void**>(m_audPlaceableTracker) = reinterpret_cast<void*>(Core::Rebase(0xD69690));
    *reinterpret_cast<uint32*>(m_audPlaceableTracker + 8) = 0;

    Core::Hook::CallMethod<void>(0x9C77F0, m_vehSubObjE24);

    m_nVehUnkFF4 = -1;
    for (int32 i = 0; i < 8; ++i)
        reinterpret_cast<CVehicleLink*>(m_vehSubObjFFC + i * 0x14)->Ctor();

    Core::Hook::CallMethod<void>(0xC7E510, m_vehSubObj10E0);
    Core::Hook::CallMethod<void>(0x8B0E30, m_vehSubObj1120);

    m_nVehUnk12E0 = 0;

    uint32 flags2 = (m_nFlags2 & ~0x340u) | 0x80u;
    if ((flags2 & 0x7C00u) != 0xC00u)
        flags2 = (flags2 & ~0x7800u) | 0x400u;
    m_nFlags2 = flags2;

    m_nFlags2 |= 0x10u;
    m_nCreatedBy = createdBy;
    m_nVehUnkF9A = 0;
    m_fVehUnk10D0 = 0.0f;
    m_fVehUnk10D4 = 0.0f;
    m_fVehUnk10D8 = 0.0f;
    m_fVehUnk10DC = 0.0f;
    m_fVehUnk10C8 = 0.0f;
    m_fVehUnk10CC = 0.0f;

    m_nVehFlagsF64 = (m_nVehFlagsF64 & 0xC0u) | 0x80u;
    m_nVehFlagsF65 = 2;
    m_nVehFlagsF66 = 4;
    m_nVehFlagsF67 = 0;

    const float randNorm = static_cast<float>(Core::Hook::Call<int32>(0xD0D21A)) * Core::StaticRef<float>(0xEBE17C);
    const uint8 flagBit = (0.0f > randNorm) ? 1u : 0u;
    m_nVehFlagsF69 |= 1u;
    m_nVehFlagsF68 = (flagBit & 1u) | 0x80u;
    m_nVehFlagsF69 &= 0x8Fu;
    m_nVehFlagsF69 &= 0xBFu;
    m_nVehFlagsF6E &= 0x32u;
    m_nVehFlagsF69 &= 0x71u;
    m_nVehFlagsF64 &= 0xBFu;
    m_nFlags2 |= 0x4000000u;

    m_nVehUnk250 = 0;
    m_nVehFlagsF6A = 0x90u;
    m_nVehFlagsF6B = (m_nVehFlagsF6B & 0x20u) | 0x11u;
    m_nVehFlagsF6C = 0;
    m_nVehFlagsF6D = 0x20u;
    m_nVehFlagsF6F = 8u;
    m_nVehFlagsF70 = 4u;
    m_nVehFlagsF71 = (m_nVehFlagsF71 & 0xD2u) | 0x12u;

    Core::Hook::Call<void>(0x83B3E0, this, 0);

    m_nVehUnk1110 = 0;
    m_nVehUnk1114 = 0;
    m_nVehUnk12F0 = 0;
    m_nVehUnk12F4 = 0;
    m_nVehUnk12F8 = 0;
    m_nVehUnk12FC = 0;

    Core::Hook::CallMethod<void>(0xA47930, this, Core::StaticRef<float>(0xDAF864));

    m_nMaxPassengers = 8;
    m_pDriver = nullptr;
    for (uint8 i = 0; i < m_nMaxPassengers; ++i)
        m_pPassengers[i] = nullptr;

    m_nVehUnk1112 &= 0x0Cu;
    m_nVehUnk10C4 = -1;
    m_nVehUnk130E = 0;
    m_nVehUnk1310 = 0;
    m_nVehUnk1314 = 0;
    m_nVehUnk1308 = 0;
    m_nVehUnk130C = 0;
    m_nVehUnk133C = 0;
    m_nVehUnk1340 = 0;
    m_nVehUnk1341 = 0;

    m_nPhysFlags118 |= 0x10u;
    m_nVehFlagsF68 &= 0xF7u;

    const float defaultF = Core::StaticRef<float>(0xDCAFF0);
    const float oneF = Core::StaticRef<float>(0xD50398);
    m_fVehUnk1304 = defaultF;
    m_fVehUnk131C = defaultF;
    m_fVehUnk1324 = defaultF;
    m_fVehUnk1328 = defaultF;
    m_fVehUnk132C = oneF;

    m_nVehUnk12E4 = CTimer::ms_nTimeMs;
    m_nVehUnk12E8 = 0;
    m_nVehUnk12EC = 0;
    m_nVehUnk1300 = 0;
    m_nVehUnkF98 = 0;
    m_nVehDoorLockState1320 = 1;
    m_fVehUnk1318 = 0.0f;
    m_nVehUnk1335 = 0;
    m_nVehUnk1336 = 0;
    m_nVehUnk1342 = 0;

    *reinterpret_cast<uint32*>(m_audPlaceableTracker + 0x0C) = 0;
    *reinterpret_cast<uint32*>(m_audPlaceableTracker + 0x10) = 0;
    *reinterpret_cast<uint32*>(m_audPlaceableTracker + 0x14) = 0;
    *reinterpret_cast<uint32*>(m_audPlaceableTracker + 0x18) = 0;

    m_nVehUnkFD0 = 0;
    m_nVehUnkFD4 = 0;
    m_nVehUnkFD8 = 0;
    m_nVehUnkFDC = 0;

    const int32 idx = Core::StaticRef<int32>(0x10F9AC0);
    m_fVehUnkF84 = *reinterpret_cast<float*>(Core::Rebase(0x11F5D34) + idx * 0x210);
    m_nVehUnkF88 = 0;

    m_nVehUnk1344 = 0;
    m_nVehUnkF9C = -1;
    m_nVehUnk10C1 = 0;
    m_nVehUnk10C2 = 0;
    m_nVehUnk1343 = 0;
    m_nVehUnk1337 = 0;
    m_fVehUnk1348 = 0.0f;
    m_nVehUnk134C = 0;
    m_nVehUnk1112 &= 0xF3u;

    m_fVehUnk1118 = static_cast<float>(Core::Hook::Call<int32>(0xD0D21A) % 15);
    m_nVehUnkF74 = CTimer::ms_nTimeMs;
    m_nVehUnkFCC = 0;
    m_nVehUnkFE0 = 0;
    m_nVehUnk1338 = 0;
    m_nVehUnkF78 = -1;
    m_nVehUnkFF8 = 0;
    m_fVehUnkFC4 = 0.0f;
    m_fVehUnk1330 = oneF;
    m_nVehUnkF7C = 0;
    m_nVehUnkF80 = 0;
    m_nVehUnk110C = 0;

    for (int32 i = 0; i < 8; ++i)
        Core::Hook::CallMethod<void>(0x89BBB0, m_vehSubObjFFC + i * 0x14, this);

    m_nVehUnk109C = 0;
    m_fVehUnk10A8 = 0.0f;
    m_fVehUnk10A4 = 0.0f;
    m_fVehUnk10A0 = 0.0f;

    m_fVehUnk10B8 = 0.0f;
    m_fVehUnk10B4 = 0.0f;
    m_fVehUnk10B0 = 0.0f;
    *reinterpret_cast<CVehicle**>(m_vehSubObj1120) = this;
    Core::Hook::CallMethod<void>(0x8B3580, m_vehSubObj1120);

    m_nVehUnkFEC = 0;
    return this;
}

CPed* __thiscall CVehicle::GetPassenger(int32 seat)
{
    switch (seat)
    {
    case 0: return m_pDriver;
    case 1: return m_pPassengers[1];
    case 2: return m_pPassengers[0];
    case 3: return m_pPassengers[2];
    default: return nullptr;
    }
}

int32 __stdcall CVehicle::SeatToPassengerIndex(int32 seat)
{
    switch (seat)
    {
    case 1: return 1;
    case 2: return 0;
    case 3: return 2;
    default: return -1;
    }
}

int32 __stdcall CVehicle::PassengerIndexToSeat(int32 index)
{
    switch (index)
    {
    case 0: return 2;
    case 1: return 1;
    case 2: return 3;
    default: return 0;
    }
}

void __thiscall CVehicle::ResetSubObjAndAudio()
{
    Core::Hook::CallMethod<void>(0x8B2850, m_vehSubObj1120 + 0x10);
    Core::Hook::CallMethod<void>(0x9E22A0, this);
}

bool __thiscall CVehicle::IsFlagF6D10Set()
{
    return (m_nVehFlagsF6D & 0x10) != 0;
}

bool __thiscall CVehicle::TrySetFlagF6D10(bool updateTimer)
{
    using Fn = bool(__thiscall*)(CVehicle*);
    if (!reinterpret_cast<Fn>(m_vftable[0x12C / 4])(this))
        return false;

    if (updateTimer)
        m_nVehUnk12F4 = CTimer::ms_nTimeMs;

    m_nVehFlagsF6D |= 0x10u;
    m_nFlags &= ~8u;
    Core::Hook::Call<void>(0xA27B30, this, 0);
    return true;
}

bool __thiscall CVehicle::GetTimer12F4Progress(float* outProgress)
{
    *outProgress = Core::StaticRef<float>(0xD50398);
    if (Core::StaticRef<uint8>(0x11DC82A) == 0)
        return false;

    uint32 start = m_nVehUnk12F4;
    uint32 cur = CTimer::ms_nTimeMs;
    uint32 dur = Core::StaticRef<uint32>(0xF1D984);
    if (cur > start + dur || cur < start)
        return false;

    *outProgress = static_cast<float>(static_cast<double>(static_cast<int32>(cur - start)) / static_cast<double>(dur));
    return true;
}

bool __thiscall CVehicle::IsUsingPretendOccupants()
{
    return (m_nVehFlagsF6D & 0x40) != 0;
}

int32 __stdcall CVehicle::SeatToDoorIndex(int32 seat)
{
    switch (seat)
    {
    case 1: return 0;
    case 2: return 2;
    case 3: return 1;
    case 4: return 3;
    default: return -1;
    }
}

int32 __thiscall CVehicle::GetPedSeat(CPed* pPed)
{
    if (pPed == m_pDriver)
        return 0;
    if (pPed == m_pPassengers[0])
        return 2;
    if (pPed == m_pPassengers[1])
        return 1;
    if (pPed == m_pPassengers[2])
        return 3;
    return -1;
}

int32 __thiscall CVehicle::GetSeatForTargetId(int32 targetId)
{
    const int32 count = GetModelSeatCount();
    if (count <= 0)
        return 1;

    for (int32 i = 0; i < count; ++i)
    {
        void* entry = GetModelSeatEntry(i);
        if (Core::Hook::CallMethod<int32>(0x8EFD40, entry, 0) == targetId)
            return Core::Hook::CallMethod<int32>(0xBE6220, entry, this);
    }
    return 1;
}

int32 __thiscall CVehicle::GetAdjustedSeatIndex(int32 a1)
{
    int32 val = GetVehicleType();
    if (val != 1)
        return a1 + 1;
    if (a1 == 0)
        return 1;
    return (a1 == 2) ? 2 : 1;
}

const char* __thiscall CVehicle::GetSeatNameById(int32 id)
{
    struct Entry
    {
        const char* name;
        int32 id;
    };

    int32 vehType = GetVehicleType();
    void* mgr = Core::StaticRef<void*>(0x15F5514);
    void** mgrVtbl = *reinterpret_cast<void***>(mgr);

    using GetTableFn = Entry*(__thiscall*)(void*, int32);
    Entry* table = reinterpret_cast<GetTableFn>(mgrVtbl[3])(mgr, vehType);

    if (id >= 89 && vehType != 0)
        reinterpret_cast<void(__thiscall*)(void*, int32)>(mgrVtbl[4])(mgr, vehType);

    for (int32 i = 0; table[i].name != nullptr; ++i)
    {
        if (table[i].id == id)
            return table[i].name;
    }

    return Core::StaticRef<const char*>(0xF33A98);
}

void __thiscall CVehicle::UpdateVehUnkF84(float t)
{
    int32 idx = Core::StaticRef<int32>(0x10F9AC0);
    float baseVal = *reinterpret_cast<float*>(Core::Rebase(0x11F5D34) + idx * 0x210);
    m_fVehUnkF84 = (Core::StaticRef<float>(0xEBE004) - baseVal) * t + baseVal;
}

bool __thiscall CVehicle::IsPlayerInVehicle()
{
    CPed* pPlayer = Core::Hook::Call<CPed*>(0x817F40);
    if (!pPlayer)
        return false;
    if (pPlayer == m_pDriver)
        return true;

    for (uint32 i = 0; i < 8; ++i)
    {
        if (pPlayer == m_pPassengers[i])
            return true;
    }
    return false;
}

bool __thiscall CVehicle::HasDummyOccupants()
{
    if (m_pDriver != nullptr && m_pDriver->m_nDummyConvertState == 2)
        return true;

    for (int32 i = 0; i < 8; ++i)
    {
        if (m_pPassengers[i] != nullptr && m_pPassengers[i]->m_nDummyConvertState == 2)
            return true;
    }
    return false;
}

bool __thiscall CVehicle::HasAnyOccupants()
{
    if ((m_nVehFlagsF6D & 0x40) != 0)
        return true;
    if (m_pDriver != nullptr)
        return true;

    for (int32 i = 0; i < 8; ++i)
    {
        if (m_pPassengers[i] != nullptr)
            return true;
    }
    return false;
}

bool __thiscall CVehicle::HasAnyUninjuredOccupants()
{
    if ((m_nVehFlagsF6D & 0x40) != 0)
        return true;
    if (m_pDriver != nullptr && m_pDriver->m_bIsInjured == 0)
        return true;

    for (int32 i = 0; i < 8; ++i)
    {
        if (m_pPassengers[i] != nullptr && m_pPassengers[i]->m_bIsInjured == 0)
            return true;
    }
    return false;
}

void* __thiscall CVehicle::FindSubObjFD0(int32 key)
{
    int32 count = static_cast<int32>(m_nVehUnkFD4);
    if (count <= 0)
        return nullptr;

    uint8* arr = reinterpret_cast<uint8*>(m_nVehUnkFD0);
    for (int32 i = 0; i < count; ++i)
    {
        if (*reinterpret_cast<int32*>(arr + i * 0x170) == key)
            return arr + i * 0x170;
    }
    return nullptr;
}

int32 __thiscall CVehicle::CountActiveSubObjFD0()
{
    void* pSub = Core::Hook::CallMethod<void*>(0x9E7080, this);
    bool flag = false;
    if (!pSub)
    {
        flag = true;
    }
    else
    {
        void* p18 = *reinterpret_cast<void**>(reinterpret_cast<uint8*>(pSub) + 0x18);
        if (p18 != nullptr)
        {
            uint32 v = *reinterpret_cast<uint32*>(reinterpret_cast<uint8*>(p18) + 0x18);
            if (v == 0 || v == 2)
                flag = true;
        }
    }

    int32 count = static_cast<int32>(m_nVehUnkFD4);
    int32 total = 0;
    if (count > 0)
    {
        uint8* p = reinterpret_cast<uint8*>(m_nVehUnkFD0) + 0x164;
        for (int32 i = 0; i < count; ++i)
        {
            uint32 f = *reinterpret_cast<uint32*>(p);
            if ((f & 1u) != 0 || (flag && ((f >> 1) & 1u) != 0))
                ++total;
            p += 0x170;
        }
    }
    return total;
}

void* __thiscall CVehicle::FindSubObjFD8(int32 key)
{
    int32 count = static_cast<int32>(m_nVehUnkFDC);
    if (count <= 0)
        return nullptr;

    uint8* arr = reinterpret_cast<uint8*>(m_nVehUnkFD8);
    for (int32 i = 0; i < count; ++i)
    {
        if (*reinterpret_cast<int32*>(arr + i * 0x34) == key)
            return arr + i * 0x34;
    }
    return nullptr;
}

void __cdecl CVehicle::GetModelBounds(int32 modelIdx, float* outA, float* outB, uint8 a4)
{
    CBaseModelInfo* modelInfo = CModelInfo::ms_modelInfoPtrs[modelIdx];
    Core::Hook::CallMethod<void>(0xBE63A0, modelInfo, outA, outB, a4);
}

void __thiscall CVehicle::UpdateTimer130E()
{
    uint16 val = static_cast<uint16>(m_nVehUnk130E);
    if (val != 0)
    {
        float dt = CTimer::ms_fFrameTimeStep;
        uint32 dec = static_cast<uint32>(static_cast<int64>(dt * Core::StaticRef<float>(0xD82154)));
        if (static_cast<uint32>(val) > dec)
            m_nVehUnk130E = static_cast<int16>(val - dec);
        else
            m_nVehUnk130E = 0;

        if (static_cast<uint16>(m_nVehUnk130E) == 0)
        {
            using Fn = void(__thiscall*)(CVehicle*, uint32, int32, int32, int32, int32);
            reinterpret_cast<Fn>(m_vftable[0x180 / 4])(this, m_nVehUnk1310, 0, 1, 0, 0x33);
        }
    }
}

bool __stdcall CVehicle::IsObjectFlagSet(const uint8* pObj)
{
    if (!pObj)
        return false;

    bool flag = false;
    if (Core::StaticRef<int32>(0x10F8044) == 2)
    {
        CPed* pPlayer = Core::Hook::Call<CPed*>(0x817F40);
        if (pPlayer != nullptr && *reinterpret_cast<const int32*>(reinterpret_cast<const uint8*>(pPlayer) + 0xEF4) > 0)
            flag = true;
    }

    if (static_cast<uint8>(pObj[0x295E] ^ pObj[0x295C]) > 0x7Fu)
        return true;

    if (Core::StaticRef<uint8>(0x1724286) != 0 || flag)
        return false;

    return static_cast<uint8>(pObj[0x296E] ^ pObj[0x296C]) > 0x7Fu;
}

void __thiscall CVehicle::RemovePassenger(CPed* pPed)
{
    if (!pPed)
        return;

    int32 idx = 0;
    if (GetVehicleSubType() == 3)
    {
        for (idx = 0; idx < 8; ++idx)
        {
            if (m_pPassengers[idx] == pPed)
                break;
        }
        if (idx >= 8)
            return;
    }
    else
    {
        int32 maxPass = static_cast<int32>(m_nMaxPassengers);
        if (maxPass <= 0)
            return;

        for (idx = 0; idx < maxPass; ++idx)
        {
            if (m_pPassengers[idx] == pPed)
                break;
        }
        if (idx >= maxPass)
            return;
    }

    CPed** slot = &m_pPassengers[idx];
    if (*slot != nullptr)
        (*slot)->CleanUpOldReference(reinterpret_cast<CEntity**>(slot));
    *slot = nullptr;
}

void* __thiscall CVehicle::GetModelSeatEntry(int32 idx)
{
    uint8* modelInfo = reinterpret_cast<uint8*>(CModelInfo::ms_modelInfoPtrs[m_nModelIndex]);
    return modelInfo + 12 * idx + 0x350;
}

int32 __thiscall CVehicle::GetModelSeatCount()
{
    const uint8* modelInfo = reinterpret_cast<const uint8*>(CModelInfo::ms_modelInfoPtrs[m_nModelIndex]);
    return *reinterpret_cast<const int32*>(modelInfo + 0x3BC);
}

bool __thiscall CVehicle::HasAnyActiveLink()
{
    uint32 count = m_nVehUnkFF8;
    if (count == 0)
        return false;

    for (uint32 i = 0; i < count; ++i)
    {
        if (reinterpret_cast<CVehicleLink*>(m_vehSubObjFFC + i * 0x14)->HasAnyEntity())
            return true;
    }
    return false;
}

int32 __thiscall CVehicle::FindLinkIndexByKey(int32 key)
{
    uint32 count = m_nVehUnkFF8;
    if (count == 0)
        return -1;

    for (uint32 i = 0; i < count; ++i)
    {
        if (*reinterpret_cast<const int32*>(m_vehSubObjFFC + i * 0x14 + 0xC) == key)
            return static_cast<int32>(i);
    }
    return -1;
}

void* __thiscall CVehicle::GetLink(int32 idx)
{
    return m_vehSubObjFFC + idx * 0x14;
}

void* __thiscall CVehicle::AllocateLink(int32 val)
{
    CVehicleLink* entry = reinterpret_cast<CVehicleLink*>(m_vehSubObjFFC + m_nVehUnkFF8 * 0x14);
    entry->SetBoneId(val);
    ++m_nVehUnkFF8;
    return entry;
}

void __cdecl CVehicle::InitVehicleConfig()
{
    if (Core::StaticRef<uint8>(0x11E917C) == 0)
        return;

    void* pCfg = reinterpret_cast<void*>(Core::Rebase(0x11E916C));
    Core::Hook::CallMethod<void>(0x88A3E0, reinterpret_cast<void*>(Core::Rebase(0x16192D0)), pCfg, reinterpret_cast<const char*>(Core::Rebase(0xD82084)), reinterpret_cast<void*>(Core::Rebase(0xD81D1F)));
    Core::Hook::CallMethod<void>(0x88A3E0, reinterpret_cast<void*>(Core::Rebase(0x1619290)), pCfg, reinterpret_cast<const char*>(Core::Rebase(0xD82094)), reinterpret_cast<void*>(Core::Rebase(0xD81E69)));
    Core::Hook::CallMethod<void>(0x88A3E0, reinterpret_cast<void*>(Core::Rebase(0x16192B0)), pCfg, reinterpret_cast<const char*>(Core::Rebase(0xD820A0)), reinterpret_cast<void*>(Core::Rebase(0xD81E6A)));
    Core::Hook::CallMethod<void>(0x88A3E0, reinterpret_cast<void*>(Core::Rebase(0x1619270)), pCfg, reinterpret_cast<const char*>(Core::Rebase(0xD820AC)), reinterpret_cast<void*>(Core::Rebase(0xD81E6B)));
    Core::Hook::CallMethod<void>(0x88A3E0, reinterpret_cast<void*>(Core::Rebase(0x16192F0)), pCfg, reinterpret_cast<const char*>(Core::Rebase(0xD820BC)), reinterpret_cast<void*>(Core::Rebase(0xD81E75)));

    Core::StaticRef<float>(0x1619120) = Core::Hook::CallMethod<float>(0x88A720, pCfg, "car.headlight.angle", 0.0f);
    Core::Hook::CallMethod<void>(0x88A750, reinterpret_cast<void*>(Core::Rebase(0x16191C8)), pCfg);
    Core::StaticRef<float>(0xF33A44) = Core::Hook::CallMethod<float>(0x88A720, pCfg, "car.lod.distance.high", 0.0f);
    Core::StaticRef<float>(0xF33A48) = Core::Hook::CallMethod<float>(0x88A720, pCfg, "car.lod.distance.low", 0.0f);
    Core::Hook::Call<void>(0x88C880);
}

int32 __thiscall CVehicle::GetPassengerCount()
{
    int32 maxPass = static_cast<int32>(m_nMaxPassengers);
    int32 count = 0;
    if (maxPass > 0)
    {
        for (int32 i = 0; i < maxPass; ++i)
        {
            if (m_pPassengers[i] != nullptr)
                ++count;
        }
    }
    return count;
}

void __thiscall CVehicle::SetOccupantsPropFlag100(bool setFlag)
{
    auto updatePedFlag = [setFlag](CPed* pPed)
    {
        if (pPed != nullptr)
        {
            if (setFlag)
                pPed->m_nPropFlags |= 0x100u;
            else
                pPed->m_nPropFlags &= ~0x100u;
        }
    };

    updatePedFlag(m_pDriver);
    for (int32 i = 0; i < 8; ++i)
        updatePedFlag(m_pPassengers[i]);
}

void __thiscall CVehicle::AddVehUnk10A0(const float* pVec3)
{
    m_fVehUnk10A0 += pVec3[0];
    m_fVehUnk10A4 += pVec3[1];
    m_fVehUnk10A8 += pVec3[2];
}

void __thiscall CVehicle::AddVehUnk10B0(const float* pVec3)
{
    m_fVehUnk10B0 += pVec3[0];
    m_fVehUnk10B4 += pVec3[1];
    m_fVehUnk10B8 += pVec3[2];
}

void __thiscall CVehicle::AddCrossVehUnk10B0(const float* pA, const float* pB)
{
    m_fVehUnk10B0 += pA[2] * pB[1] - pA[1] * pB[2];
    m_fVehUnk10B4 += pA[0] * pB[2] - pA[2] * pB[0];
    m_fVehUnk10B8 += pA[1] * pB[0] - pA[0] * pB[1];
}

bool __thiscall CVehicle::IsAudioSubTypeF()
{
    uint8* pSub = Core::Hook::Call<uint8*>(0xA26FC0, this);
    return pSub != nullptr && pSub[0x26] == 0xF;
}

void __thiscall CVehicle::ProcessMovingAudio1348()
{
    float vec[4] = {};
    using GetVelFn = void(__thiscall*)(CVehicle*, float*);
    reinterpret_cast<GetVelFn>(m_vftable[0xEC / 4])(this, vec);

    if (m_nVehUnk134C != 0 && (vec[1] * vec[1] + vec[0] * vec[0]) > Core::StaticRef<float>(0xDCB2A0))
    {
        m_nVehUnk134C = 0;
        m_fVehUnk1348 += CTimer::ms_fFrameTimeStep;
        if (m_fVehUnk1348 >= Core::StaticRef<float>(0xF33AA8))
        {
            uint32 arg1 = reinterpret_cast<uint32>(&m_pMatrix->pos);
            Core::Hook::Call<void>(0x987730, 23, arg1, this, 0, 0, 0.0f);
        }
    }
    else
    {
        m_fVehUnk1348 = 0.0f;
    }
}

void __thiscall CVehicle::ProcessOccupantDamageF67()
{
    if ((m_nVehFlagsF67 & 8) == 0)
        return;

    auto applyToPed = [this](CPed* pPed)
    {
        if (!pPed)
            return;

        uint8* p224 = static_cast<uint8*>(pPed->m_pPedIntelligence);
        uint32* node = *reinterpret_cast<uint32**>(p224 + 0x2E0);
        if (node != nullptr)
        {
            uint32 prevPri = (node[2] >> 1) & 7u;
            do
            {
                uint32 pri = (node[2] >> 1) & 7u;
                if (prevPri < pri)
                    break;
                prevPri = pri;
                if (node[1] == 0x32D)
                    return;
                node = reinterpret_cast<uint32*>(node[3]);
            } while (node);
        }

        uint8 eventBuf[0x24] = {};
        Core::Hook::CallMethod<void>(0xAB9A00, eventBuf, this, CTimer::ms_nTimeMs, 0x3A);

        uint8 dmgInfo[0x14] = {};
        float dmg = CTimer::ms_fFrameTimeStep * Core::StaticRef<float>(0xD81E7C);
        Core::Hook::CallMethod<void>(0xAB9470, dmgInfo, this, dmg, 0x3A, 0, 0);

        uint8 resp[0x18] = {};
        Core::Hook::CallMethod<void>(0xABB170, dmgInfo, pPed, resp);

        if ((resp[4] & 0xC) != 0)
            Core::Hook::CallMethod<void>(0xAA80D0, p224 + 0x84, eventBuf, 0, 1);

        Core::Hook::CallMethod<void>(0xC28820, dmgInfo);
        Core::Hook::CallMethod<void>(0xAB9A80, eventBuf);
    };

    applyToPed(m_pDriver);

    int32 maxPass = static_cast<int32>(m_nMaxPassengers);
    if (maxPass > 0)
    {
        for (int32 i = 0; i < maxPass; ++i)
            applyToPed(m_pPassengers[i]);
    }
}

int32 __thiscall CVehicle::GetDrawHandlerUnkD8()
{
    uint8* p34 = static_cast<uint8*>(m_pDrawHandler);
    if (!p34)
        return -1;
    uint8* p4 = *reinterpret_cast<uint8**>(p34 + 4);
    return p4 ? *reinterpret_cast<int32*>(p4 + 0xD8) : -1;
}

void __thiscall CVehicle::SetDrawHandlerUnkD8(int32 val)
{
    uint8* p34 = static_cast<uint8*>(m_pDrawHandler);
    if (!p34)
        return;
    uint8* p4 = *reinterpret_cast<uint8**>(p34 + 4);
    if (p4)
        *reinterpret_cast<int32*>(p4 + 0xD8) = val;
}

void __thiscall CVehicleStateSnapshot::Save(CVehicle* pVeh)
{
    uint8* base = reinterpret_cast<uint8*>(this);
    const float* mat = reinterpret_cast<const float*>(pVeh->m_pMatrix);
    float* dstMat = reinterpret_cast<float*>(base);
    for (int32 row = 0; row < 4; ++row)
    {
        dstMat[row * 4 + 0] = mat[row * 4 + 0];
        dstMat[row * 4 + 1] = mat[row * 4 + 1];
        dstMat[row * 4 + 2] = mat[row * 4 + 2];
    }

    std::memcpy(base + 0x41, reinterpret_cast<const uint8*>(pVeh) + 0xFE4, 4);
    *reinterpret_cast<int16*>(base + 0x46) = pVeh->m_nVehUnkF98;
    base[0x48] = pVeh->m_nMaxPassengers;
    *reinterpret_cast<float*>(base + 0x4C) = pVeh->m_fVehUnk10D8;
    *reinterpret_cast<float*>(base + 0x50) = pVeh->m_fVehUnk10C8;
    *reinterpret_cast<float*>(base + 0x54) = pVeh->m_fVehUnk10CC;
    std::memcpy(base + 0x58, &pVeh->m_nVehFlagsF64, 0xE);
}

void __thiscall CVehicleStateSnapshot::Restore(CVehicle* pVeh)
{
    const uint8* base = reinterpret_cast<const uint8*>(this);
    using Fn1 = void(__thiscall*)(CVehicle*, void*, int32, int32);
    reinterpret_cast<Fn1>(pVeh->m_vftable[1])(pVeh, this, 0, 0);

    std::memcpy(reinterpret_cast<uint8*>(pVeh) + 0xFE4, base + 0x41, 4);
    pVeh->ResetDrawHandler();

    pVeh->m_nVehUnkF98 = *reinterpret_cast<const int16*>(base + 0x46);
    pVeh->m_nMaxPassengers = base[0x48];
    pVeh->m_fVehUnk10D8 = *reinterpret_cast<const float*>(base + 0x4C);
    pVeh->m_fVehUnk10C8 = *reinterpret_cast<const float*>(base + 0x50);
    pVeh->m_fVehUnk10CC = *reinterpret_cast<const float*>(base + 0x54);
    std::memcpy(&pVeh->m_nVehFlagsF64, base + 0x58, 0xE);
}

void* __thiscall CVehicle::GetBoneLocalMatrix(int32 boneIdx)
{
    void* v40 = reinterpret_cast<void*(__thiscall*)(CVehicle*)>(m_vftable[40])(this);
    uint8* pObj = v40
        ? reinterpret_cast<uint8*(__thiscall*)(void*)>((*reinterpret_cast<void***>(v40))[56])(v40)
        : *reinterpret_cast<uint8**>(reinterpret_cast<uint8*>(this) + 0x100);

    if (pObj != nullptr)
    {
        uint8* p78 = *reinterpret_cast<uint8**>(reinterpret_cast<uint8*>(this) + 0x78);
        if (p78 != nullptr && (p78[0] & 1) != 0)
            Core::Hook::CallMethod<void>(0x509FF0, reinterpret_cast<void*>(Core::Rebase(0x16397D0)), p78 + 0x1A04, 1);

        return *reinterpret_cast<uint8**>(pObj + 0xC) + 0x50 * boneIdx;
    }

    float one = Core::StaticRef<float>(0xD50398);
    Core::StaticRef<float>(0x1639460) = one;
    Core::StaticRef<float>(0x1639474) = one;
    Core::StaticRef<float>(0x1639488) = one;
    Core::StaticRef<float>(0x1639464) = 0.0f;
    Core::StaticRef<float>(0x1639468) = 0.0f;
    Core::StaticRef<float>(0x1639470) = 0.0f;
    Core::StaticRef<float>(0x1639478) = 0.0f;
    Core::StaticRef<float>(0x1639480) = 0.0f;
    Core::StaticRef<float>(0x1639484) = 0.0f;
    Core::StaticRef<float>(0x1639490) = 0.0f;
    Core::StaticRef<float>(0x1639494) = 0.0f;
    Core::StaticRef<float>(0x1639498) = 0.0f;
    return reinterpret_cast<void*>(Core::Rebase(0x1639460));
}

int32 __thiscall CVehicleLink::GetBoneIndex()
{
    return m_pOwner->FindLinkIndexByKey(m_nBoneId);
}

void __thiscall CVehicleLink::UpdateAudio(bool bUsePending)
{
    CVehicle* pA = m_pOwner;
    CEntity* pB = static_cast<CEntity*>(bUsePending ? m_pPending : m_pAttached);
    uint8 argFlag = bUsePending ? 1 : 0;

    if (pA != nullptr && pB != nullptr)
    {
        void* p6Ca = reinterpret_cast<void*>(pA->m_nUnk6C);
        void* p6Cb = reinterpret_cast<void*>(pB->m_nUnk6C);
        if (p6Ca != nullptr && p6Cb != nullptr)
        {
            int32 bone = GetBoneIndex();
            int32 idB = Core::Hook::CallMethod<int32>(0x49F370, p6Cb);
            int32 idA = Core::Hook::CallMethod<int32>(0x49F370, p6Ca);
            Core::Hook::CallMethod<void>(0x470980, reinterpret_cast<void*>(Core::Rebase(0x18CF5C8)), idA, idB, bone, argFlag, -1);
            m_nTimer = 200;
            m_bFlag = 1;
        }
    }
}

void __thiscall CVehicleLink::Set(void* pEntity, bool bSet, bool bDoUpdate)
{
    if (bDoUpdate)
    {
        CEntity* pEnt = static_cast<CEntity*>(pEntity);
        if (bSet)
            SetAttached(pEnt);
        else if (IsAttached(pEnt))
            SetAttached(nullptr);
    }
    m_nTimer = 0;
    m_bFlag = 0;
}

bool __thiscall CVehicleLink::Attach(void* pEntity)
{
    CEntity* pEnt = static_cast<CEntity*>(pEntity);
    uint8* p6C = reinterpret_cast<uint8*>(pEnt->m_nUnk6C);
    if (!p6C || p6C[0xE] == 0)
    {
        m_pPending = pEntity;
        pEnt->RegisterReference(reinterpret_cast<CEntity**>(&m_pPending));
    }

    uint8* pOwner6C = reinterpret_cast<uint8*>(m_pOwner->m_nUnk6C);
    if (pOwner6C != nullptr && (pOwner6C[0xE] != 0 || Core::Hook::CallMethod<bool>(0x49F3F0, pOwner6C)))
        return false;

    m_pAttached = pEntity;
    pEnt->RegisterReference(reinterpret_cast<CEntity**>(&m_pAttached));
    return true;
}

bool __thiscall CVehicleLink::Detach(void* pEntity)
{
    if (m_pPending == pEntity)
    {
        if (m_pPending)
            static_cast<CEntity*>(m_pPending)->CleanUpOldReference(reinterpret_cast<CEntity**>(&m_pPending));
        m_pPending = nullptr;
    }

    uint8* pOwner6C = reinterpret_cast<uint8*>(m_pOwner->m_nUnk6C);
    if (pOwner6C != nullptr && (pOwner6C[0xE] != 0 || Core::Hook::CallMethod<bool>(0x49F3F0, pOwner6C)))
        return false;

    if (m_pAttached)
        static_cast<CEntity*>(m_pAttached)->CleanUpOldReference(reinterpret_cast<CEntity**>(&m_pAttached));
    m_pAttached = nullptr;
    return true;
}

void __thiscall CVehicleLink::Process()
{
    uint8* pOwner6C = reinterpret_cast<uint8*>(m_pOwner->m_nUnk6C);
    bool canModify = (!pOwner6C || pOwner6C[0xE] == 0) && (!pOwner6C || !Core::Hook::CallMethod<bool>(0x49F3F0, pOwner6C));

    if (m_nTimer > 0)
    {
        m_nTimer -= static_cast<int16>(static_cast<int32>(CTimer::ms_fFrameTimeStep * Core::StaticRef<float>(0xDAF864)));
        if (m_nTimer <= 0)
        {
            m_nTimer = 0;
            m_bFlag = 0;
        }
    }

    if (m_pPending != nullptr)
    {
        CEntity* pPendEnt = static_cast<CEntity*>(m_pPending);
        uint8* pPend6C = reinterpret_cast<uint8*>(pPendEnt->m_nUnk6C);
        if ((!pPend6C || pPend6C[0xE] == 0) && m_pAttached == nullptr)
        {
            if (canModify)
            {
                m_pAttached = m_pPending;
                pPendEnt->RegisterReference(reinterpret_cast<CEntity**>(&m_pAttached));
                m_bFlag = 0;
            }
            else if (m_bFlag == 0)
            {
                UpdateAudio(true);
            }
        }

        if (m_pPending)
            static_cast<CEntity*>(m_pPending)->CleanUpOldReference(reinterpret_cast<CEntity**>(&m_pPending));
        m_pPending = nullptr;
    }
    else if (m_pAttached != nullptr)
    {
        CEntity* pAttEnt = static_cast<CEntity*>(m_pAttached);
        uint8* pAtt6C = reinterpret_cast<uint8*>(pAttEnt->m_nUnk6C);
        if (!pAtt6C || pAtt6C[0xE] == 0)
        {
            if (canModify)
            {
                pAttEnt->CleanUpOldReference(reinterpret_cast<CEntity**>(&m_pAttached));
                m_pAttached = nullptr;
            }
            else if (m_bFlag == 0)
            {
                UpdateAudio(false);
            }
        }
    }
}

CVehicleSubObjE24* __thiscall CVehicleSubObjE24::Ctor()
{
    uint8* base = reinterpret_cast<uint8*>(this);
    for (int32 i = 0; i < 14; ++i)
        reinterpret_cast<int32*>(base)[i] = -1;

    Core::Hook::CallMethod<void>(0xA34470, base + 0x74);
    Core::Hook::CallMethod<void>(0xA34470, base + 0xA0);
    Core::Hook::CallMethod<void>(0xA34470, base + 0xD0);

    *reinterpret_cast<int32*>(base + 0xFC) = -1;
    Core::Hook::CallMethod<void>(0xA33360, base);

    Core::Hook::CallMethod<void>(0xA338F0, base + 0x74);
    Core::Hook::CallMethod<void>(0xA338F0, base + 0xA0);
    Core::Hook::CallMethod<void>(0xA338F0, base + 0xD0);

    base[0xCC] = 0;
    base[0x124] &= 7u;
    base[0x125] &= ~1u;
    *reinterpret_cast<uint32*>(base + 0x64) = CTimer::ms_nTimeMs;
    *reinterpret_cast<uint32*>(base + 0x68) = 0;
    base[0x11A] = 0;
    *reinterpret_cast<float*>(base + 0x11C) = Core::StaticRef<float>(0xD50398);
    base[0x13C] = 0xFF;
    base[0x119] = 0;
    *reinterpret_cast<uint32*>(base + 0x104) = 0;
    base[0x118] = 0;
    base[0x121] = 0;
    base[0x122] = 0;
    base[0x126] = 0x14;
    base[0x13D] = 0;
    base[0x127] = static_cast<uint8>((Core::Hook::Call<int32>(0xD0D21A) & 7) + 4);
    *reinterpret_cast<uint32*>(base + 0x138) = 0;
    *reinterpret_cast<float*>(base + 0x12C) = 0.0f;
    *reinterpret_cast<uint32*>(base + 0x108) = 0;
    base[0x128] = 0;
    *reinterpret_cast<uint32*>(base + 0x10C) = 0;
    *reinterpret_cast<uint32*>(base + 0x110) = 0;
    base[0x100] = 3;
    *reinterpret_cast<int32*>(base + 0xFC) = -1;
    *reinterpret_cast<uint32*>(base + 0x6C) = 0;
    *reinterpret_cast<float*>(base + 0x130) = 1.0f;
    *reinterpret_cast<float*>(base + 0x134) = 0.0f;
    *reinterpret_cast<uint32*>(base + 0x70) = 0;
    *reinterpret_cast<uint32*>(base + 0x114) = 0;
    base[0x123] = 0;
    return this;
}

void __cdecl CVehicle::InitSystem()
{
    Core::Hook::Call<void>(0x9718C0);
    CVehicle::InitVehicleConfig();
    Core::StaticRef<uint32>(0x1619258) = Core::Hook::Call<uint32>(0x5B1C30, "headlights", 0);
    Core::StaticRef<uint32>(0x161925C) = Core::Hook::Call<uint32>(0x5B1C30, "carbottom", 0);
    Core::StaticRef<uint32>(0x1619260) = Core::Hook::Call<uint32>(0x5B1C30, "bikebottom", 0);
}

int32 __thiscall CVehicle::ProcessControlStub(int32, int32, int32, int32, int32)
{
    return 0;
}

int32 __thiscall CVehicle::GetDefaultMaxPassengers()
{
    return 10;
}

int32 __thiscall CVehicle::PreRender()
{
    CVehicleModelInfo* modelInfo = static_cast<CVehicleModelInfo*>(CModelInfo::ms_modelInfoPtrs[m_nModelIndex]);
    int32* pCC = reinterpret_cast<int32*>(modelInfo->m_pVehicleStruct);

    auto zeroBoneMat = [this](int32 bone)
    {
        float* m = Core::Hook::CallMethod<float*>(0x4D9100, this, bone);
        m[0] = m[1] = m[2] = 0.0f;
        m[4] = m[5] = m[6] = 0.0f;
        m[8] = m[9] = m[10] = 0.0f;
        m[12] = m[13] = m[14] = 0.0f;
    };

    if (m_nVehBoneMaskE20 != 0)
    {
        for (int32 i = 0x59; i <= 0x62; ++i)
        {
            if (((1u << (i - 0x58)) & m_nVehBoneMaskE20) != 0)
                zeroBoneMat(pCC[i]);
        }
    }

    uint8* self = reinterpret_cast<uint8*>(this);
    if (m_bVehBoneResetA != 0)
    {
        for (int32 k = 0; k <= 12; ++k)
        {
            if (m_nVehBoneResetMaskA[k] != 0)
                zeroBoneMat(*reinterpret_cast<int32*>(reinterpret_cast<uint8*>(pCC) + 0xC8 + k * 4));
        }
        m_bVehBoneResetA = 0;
    }

    if ((m_nVehFlagsF6E & 0x20) != 0 && m_bVehBoneResetB != 0)
    {
        for (int32 k = 0; k <= 7; ++k)
        {
            if (m_nVehBoneResetMaskB[k] != 0)
                zeroBoneMat(*reinterpret_cast<int32*>(reinterpret_cast<uint8*>(pCC) + 0x104 + k * 4));
        }
        m_bVehBoneResetB = 0;
    }

    if (Core::StaticRef<uint8>(0x15BFF22) == 0)
    {
        float scale = m_fVehFadeScale111C;
        if (scale >= 0.0f)
        {
            self[0x5B] = static_cast<uint8>(static_cast<int32>(static_cast<float>(self[0x5B]) * scale));
            m_fVehFadeScale111C = Core::StaticRef<float>(0xEBDED4);
        }
    }

    modelInfo->m_nIDEFlags |= 0x2000000u;

    bool hasPlayer = IsPlayerInVehicle();
    if (hasPlayer)
        m_nFlags2 |= 0x10000000u;
    else
        m_nFlags2 &= ~0x10000000u;

    bool bit29 = hasPlayer && ((Core::StaticRef<uint8>(0x11DC582) & 2) != 0 || Core::StaticRef<uint8>(0x1111D88) != 0);
    if (bit29)
        m_nFlags2 |= 0x20000000u;
    else
        m_nFlags2 &= ~0x20000000u;

    bool bit28 = (m_nFlags2 & 0x10000000u) != 0;
    auto propagateBit28 = [bit28](CPed* pPed)
    {
        if (pPed != nullptr)
        {
            if (bit28)
                pPed->m_nFlags2 |= 0x10000000u;
            else
                pPed->m_nFlags2 &= ~0x10000000u;
        }
    };

    propagateBit28(m_pDriver);
    int32 maxPass = static_cast<int32>(m_nMaxPassengers);
    if (maxPass > 0)
    {
        for (int32 i = 0; i < maxPass; ++i)
            propagateBit28(m_pPassengers[i]);
    }

    if (Core::StaticRef<int32>(0x10D0170) == 1 ||
        Core::StaticRef<int32>(0x10E1984) != Core::StaticRef<int32>(0xE3447C) ||
        Core::StaticRef<int32>(0xF0E4A0) == 0x12)
    {
        return 0;
    }

    return Core::Hook::CallMethod<int32>(0x9E1480, this);
}

void* __thiscall CVehicle::CreateModelInstance(int32 a2, int32 a3, int32 a4, int32 a5)
{
    if ((m_nFlags2 & 0x20000000u) != 0 && a2 == 1 && a3 != 0x40)
        return nullptr;

    int32 modelIdx = m_nModelIndex;
    CVehicleModelInfo* modelInfo = static_cast<CVehicleModelInfo*>(CModelInfo::ms_modelInfoPtrs[modelIdx]);
    int32 effModel = (a5 != -1) ? a5 : modelIdx;
    void* savedPtr = nullptr;

    if ((m_nVehFlagsF70 & 0x40) != 0)
    {
        int32 altModel = *reinterpret_cast<const int32*>(reinterpret_cast<const uint8*>(modelInfo) + 0x34C);
        if (Core::Hook::Call<bool>(0x832DD0, altModel, CModelInfo::ms_nStreamingModuleId))
        {
            effModel = altModel;
            Core::Hook::CallMethod<void>(0x7B7130, reinterpret_cast<void*>(Core::Rebase(0x10F5F80)), altModel);
            uint8* p34 = static_cast<uint8*>(m_pDrawHandler);
            savedPtr = p34 ? *reinterpret_cast<void**>(p34 + 4) : nullptr;
            if (p34)
                *reinterpret_cast<uint32*>(p34 + 4) = m_nVehUnkFEC;
        }
    }

    auto makeDC = [](uintptr_t fnAddr)
    {
        uint32* dc = Core::Hook::Call<uint32*>(0x7BDD80, 12, 0);
        if (dc)
        {
            dc[0] = Core::Rebase(0xD50380);
            uint32& seq = Core::StaticRef<uint32>(0xF1023C);
            dc[1] = (dc[1] & ~0x3FFFu) | (seq++ & 0x3FFFu);
            dc[0] = Core::Rebase(0xD503A0);
            dc[2] = Core::Rebase(fnAddr);
            Core::Hook::CallMethod<void>(0x404E90, dc);
        }
    };

    uint8* pRet = nullptr;
    if ((m_nFlags2 & 0x10000000u) != 0)
    {
        makeDC(0xB41300);
        pRet = Core::Hook::CallMethod<uint8*>(0x9E15D0, this, a2, a3, a4, effModel);
        makeDC(0xB41330);
    }
    else
    {
        pRet = Core::Hook::CallMethod<uint8*>(0x9E15D0, this, a2, a3, a4, effModel);
    }

    if (pRet != nullptr)
    {
        if (m_nVehUnk11C0 != 0 && Core::StaticRef<uint8>(0x1135B85) == 0)
            pRet[0x59] |= 0x40u;

        if (m_nVehUnkFD4 != 0)
        {
            pRet[0x59] |= 1u;
            float arrA[6] = {};
            float arrB[6] = {};
            bool any = false;
            int32 count = static_cast<int32>(m_nVehUnkFD4);
            for (int32 i = 0; i < count; ++i)
            {
                uint8* item = reinterpret_cast<uint8*>(m_nVehUnkFD0) + i * 0x170;
                float val = *reinterpret_cast<const float*>(item + 0x160);
                if (val < Core::StaticRef<float>(0xDAF864))
                {
                    float w = 1.0f;
                    if (val > 0.0f)
                    {
                        float ratio = *reinterpret_cast<const float*>(item + 0x70) / *reinterpret_cast<const float*>(item + 0x20);
                        if (ratio > 1.0f)
                            ratio = 1.0f;
                        w = (1.0f - val * Core::StaticRef<float>(0xDCB2AC)) * ratio;
                        if (w < 0.0f)
                            w = 0.0f;
                    }
                    int32 slot = *reinterpret_cast<const int32*>(item);
                    assert(slot >= 0 && slot < 6);
                    arrB[slot] = w;
                    arrA[slot] = Core::Hook::CallMethod<float>(0xB49520, item);
                    any = true;
                }
            }
            if (any)
                Core::Hook::CallMethod<void>(0x7BFE60, pRet, arrB, arrA);

            if (IsPlayerInVehicle())
                pRet[0x59] |= 8u;

            if (GetVehicleType() == 4)
            {
                uint8* pCC = reinterpret_cast<uint8*>(modelInfo->m_pVehicleStruct);
                if (*reinterpret_cast<const int16*>(pCC + 0x206) > -1 || *reinterpret_cast<const int16*>(pCC + 0x208) > -1)
                    pRet[0x59] |= (GetHeliRotorSpeed() > Core::StaticRef<float>(0xD85DB0)) ? 0x30u : 0x10u;
            }
            else if (Core::Hook::Call<bool>(0x462E60))
            {
                pRet[0x59] |= 8u;
            }
        }
    }

    if (a2 == 0 && a3 != 1 && a3 != 2)
        m_nVehUnk12E4 = CTimer::ms_nTimeMs;

    if (savedPtr)
    {
        uint8* p34 = static_cast<uint8*>(m_pDrawHandler);
        if (p34)
            *reinterpret_cast<void**>(p34 + 4) = savedPtr;
    }

    if (a3 == 8)
        ++Core::StaticRef<int32>(0x1619244);

    return pRet;
}

bool __thiscall CVehicle::GetBoneWorldPos(int32 boneIdx, float* outVec4)
{
    outVec4[0] = 0.0f;
    outVec4[1] = 0.0f;
    outVec4[2] = 0.0f;

    CVehicleModelInfo* modelInfo = static_cast<CVehicleModelInfo*>(CModelInfo::ms_modelInfoPtrs[m_nModelIndex]);
    int32 id = static_cast<int32>(modelInfo->m_pVehicleStruct->m_nBones[boneIdx]);
    if (id == -1)
        return false;

    struct BoneNode
    {
        uint8    pad00[0x10];
        BoneNode* pParent;
        uint8    pad14[0x0C];
        CVector  vecPos;
        float    f2C;
        uint8    pad30[0x10];
        uint8    quat[0x10];
    };

    void* v40 = reinterpret_cast<void*(__thiscall*)(CVehicle*)>(m_vftable[40])(this);
    uint8* pObj = v40
        ? reinterpret_cast<uint8*(__thiscall*)(void*)>((*reinterpret_cast<void***>(v40))[56])(v40)
        : *reinterpret_cast<uint8**>(reinterpret_cast<uint8*>(this) + 0x100);

    uint8* base = **reinterpret_cast<uint8***>(pObj + 4) + id * 0xE0;
    outVec4[0] = *reinterpret_cast<const float*>(base + 0x20);
    outVec4[1] = *reinterpret_cast<const float*>(base + 0x24);
    outVec4[2] = *reinterpret_cast<const float*>(base + 0x28);
    outVec4[3] = *reinterpret_cast<const float*>(base + 0x2C);

    for (BoneNode* node = *reinterpret_cast<BoneNode**>(**reinterpret_cast<uint8***>(pObj + 4) + id * 0xE0 + 0x10);
         node != nullptr;
         node = node->pParent)
    {
        CMatrix mat;
        Core::Hook::CallMethod<void>(0x5BE050, &mat, node->quat);
        CVector v(outVec4[0], outVec4[1], outVec4[2]);
        outVec4[0] = mat.right.x * v.x + mat.up.x * v.y + mat.at.x * v.z + node->vecPos.x;
        outVec4[1] = mat.right.y * v.x + mat.up.y * v.y + mat.at.y * v.z + node->vecPos.y;
        outVec4[2] = mat.right.z * v.x + mat.up.z * v.y + mat.at.z * v.z + node->vecPos.z;
    }

    return true;
}

void __thiscall CVehicle::GetBoneMatrix(int32 boneId, float* outMat4x4)
{
    const float* m = static_cast<const float*>(GetBoneLocalMatrix(boneId));
    for (int32 row = 0; row < 4; ++row)
    {
        outMat4x4[row * 4 + 0] = m[row * 4 + 0];
        outMat4x4[row * 4 + 1] = m[row * 4 + 1];
        outMat4x4[row * 4 + 2] = m[row * 4 + 2];
    }

    void* v40 = reinterpret_cast<void*(__thiscall*)(CVehicle*)>(m_vftable[40])(this);
    uint8* pObj = v40
        ? reinterpret_cast<uint8*(__thiscall*)(void*)>((*reinterpret_cast<void***>(v40))[56])(v40)
        : *reinterpret_cast<uint8**>(reinterpret_cast<uint8*>(this) + 0x100);

    for (uint8* node = *reinterpret_cast<uint8**>(**reinterpret_cast<uint8***>(pObj + 4) + boneId * 0xE0 + 0x10);
         node != nullptr;
         node = *reinterpret_cast<uint8**>(node + 0x10))
    {
        const float* pm = static_cast<const float*>(GetBoneLocalMatrix(static_cast<int32>(*reinterpret_cast<const uint16*>(node + 0x16))));
        float tmp[16] = {};
        for (int32 row = 0; row < 4; ++row)
        {
            tmp[row * 4 + 0] = pm[row * 4 + 0];
            tmp[row * 4 + 1] = pm[row * 4 + 1];
            tmp[row * 4 + 2] = pm[row * 4 + 2];
        }
        Core::Hook::CallMethod<void>(0x513430, outMat4x4, tmp);
    }

    Core::Hook::CallMethod<void>(0x513430, outMat4x4, m_pMatrix);
}

void __thiscall CVehicle::UpdateDamageBoneTable()
{
    uint8* pE14 = static_cast<uint8*>(m_pFragInstE14);
    uint8* pC = *reinterpret_cast<uint8**>(*reinterpret_cast<uint8**>(pE14 + 4) + 0xC);
    uint32* tbl = *reinterpret_cast<uint32**>(pC + 0x94);
    if (!tbl)
        return;

    uint16 cnt92 = *reinterpret_cast<const uint16*>(pC + 0x92);
    for (int32 i = 0; i < static_cast<int32>(cnt92); ++i)
    {
        uint8* sub = Core::Hook::CallMethod<uint8*>(0x51DDD0, pE14);
        if ((*reinterpret_cast<uint8***>(sub + 0xD4))[i][0xC] != 0)
            tbl[i * 2 + 1] = 0x69F8;
    }

    int32 cntFD4 = static_cast<int32>(m_nVehUnkFD4);
    for (int32 i = 0; i < cntFD4; ++i)
    {
        int16 idx = *reinterpret_cast<const int16*>(reinterpret_cast<const uint8*>(m_nVehUnkFD0) + i * 0x170 + 4);
        if (idx > -1)
            tbl[idx * 2 + 1] = 0x3FE;
    }

    auto applyBone = [pE14, tbl](int32 boneId, uint32 val)
    {
        if (boneId != -1)
        {
            void* sub = Core::Hook::CallMethod<void*>(0x51DDD0, pE14);
            int32 res = Core::Hook::Call<int32>(0xA4B480, sub, boneId);
            if (res != -1)
                tbl[res * 2 + 1] = val;
        }
    };

    CVehicleModelInfo* modelInfo = static_cast<CVehicleModelInfo*>(CModelInfo::ms_modelInfoPtrs[m_nModelIndex]);
    uint8* pCC = reinterpret_cast<uint8*>(modelInfo->m_pVehicleStruct);

    for (uint32 off = 0x164; off <= 0x188; off += 4)
        applyBone(*reinterpret_cast<const int32*>(pCC + off), 0x7BFE);

    int32 vehSubType = GetVehicleType();
    if (vehSubType == 0)
    {
        for (uint32 off = 0x124; off <= 0x140; off += 4)
            applyBone(*reinterpret_cast<const int32*>(pCC + off), 0x7BFE);
    }
    if (vehSubType == 4)
    {
        int32 b = *reinterpret_cast<const int32*>(pCC + 0xA0);
        if (b > -1)
            applyBone(b, 0x7BFE);

        int8 c0 = GetBoatUnk1F40(0);
        if (c0 > 0)
            tbl[c0 * 2 + 1] = 0x3DE;

        int8 c1 = GetBoatUnk1F40(1);
        if (c1 > 0)
            tbl[c1 * 2 + 1] = 0x3DE;
    }
}

bool __thiscall CVehicle::CanBeConvertedToDummy(bool a2, bool a3, int32, int32, bool a6, bool a7, bool a8)
{
    if (a3)
    {
        uint8* p6C = reinterpret_cast<uint8*>(m_nUnk6C);
        if (p6C && (p6C[0xE] != 0 || !reinterpret_cast<bool(__thiscall*)(void*)>((*reinterpret_cast<void***>(p6C))[24])(p6C)))
            return false;
    }

    if (GetVehicleType() == 3)
        return false;

    if (Core::Hook::Call<bool>(0x83B940, this) && a6)
        return false;

    if (m_pDriver && ((m_pDriver->m_nDummyConvertState == 2 && a2) || (m_pDriver->m_nPedUnk219 != 0 && !a8)))
        return false;

    for (uint32 i = 0; i < 8; ++i)
    {
        if (m_pPassengers[i] && ((m_pPassengers[i]->m_nDummyConvertState == 2 && a2) || m_pPassengers[i]->m_nPedUnk219 != 0))
            return false;
    }

    if (!a7 && !Core::Hook::Call<bool>(0x439C60, this))
        return false;

    switch (m_nCreatedBy)
    {
    case 2: return !a2;
    case 4: return false;
    default: return true;
    }
}

bool __thiscall CVehicle::CanUsePretendOccupants()
{
    if (HasAliveOccupants())
        return false;
    if (m_nVehSubObjE34 == 0xFFFF)
        return false;

    uint32 tmp = 0;
    if (Core::Hook::Call<bool>(0x96A350, this, &tmp))
        return false;

    return (m_nVehFlagsF6D & 0x20) != 0;
}

bool __thiscall CVehicle::TrySpawnInWorld(bool a2, bool a3, int32 a4)
{
    if (Core::StaticRef<uint8>(0xF33AB0) != 0 && !a3)
    {
        uint8 v16[0x600];
        Core::Hook::CallStdcall<void>(0x401020, v16, 0x60, 0x10, reinterpret_cast<void*>(Core::Rebase(0x425360)));

        uint8 v14[0x40];
        Core::Hook::CallMethod<void>(0x441ED0, v14, m_pMatrix);
        *reinterpret_cast<float*>(v14 + 0x38) += Core::StaticRef<float>(0xF33AAC);

        int32 v13[2] = { reinterpret_cast<int32>(this), a4 };
        int32 cnt = Core::Hook::Call<int32>(0x99BE70, v14, this, -1, v13, 2, v16, 0x10, 0xA8);
        for (int32 i = 0; i < cnt; ++i)
        {
            CEntity* p = Core::Hook::Call<CEntity*>(0xA72220, *reinterpret_cast<const uint32*>(v16 + i * 0x60));
            if (p && (p->m_nFlags & 1u) != 0)
                return false;
        }
    }

    if (a2)
        m_nVehUnk12F4 = m_nVehUnk12F8 = CTimer::ms_nTimeMs;

    uint8* pRes = Core::Hook::CallMethod<uint8*>(0x928000, Core::StaticRef<void*>(0x1544AD0), this, 0);
    if (pRes)
    {
        reinterpret_cast<void(__thiscall*)(CVehicle*)>(m_vftable[54])(this);
        int32 h = m_nInteriorHash;
        int32 idx = m_nInteriorIndex;
        if (h != -1 && idx != -1 && idx != 0)
        {
            void* pPool = Core::Hook::CallMethod<void*>(0xA4C560, Core::StaticRef<void*>(0x165D2E4), h);
            *reinterpret_cast<uint32*>(pRes + 0x10) = pPool ? Core::Hook::CallMethod<uint32>(0xA3CA40, pPool, idx) : 0;
        }
        else
        {
            *reinterpret_cast<uint32*>(pRes + 0x10) = 0;
        }
    }

    m_nVehFlagsF6D &= ~0x10u;
    return true;
}

bool __thiscall CVehicle::IsEnterable()
{
    if (Core::StaticRef<uint8>(0xF1D926) == 0)
        return false;
    if (m_nCreatedBy == 2 || m_nCreatedBy == 3)
        return false;

    if (GetVehicleSubType() == 1)
        return false;

    int32 vt = GetVehicleType();
    if (vt == 4 || vt == 5)
        return false;

    int32 model = m_nModelIndex;
    if (model == Core::StaticRef<int32>(0x1230778) ||
        model == Core::StaticRef<int32>(0x12308EC) ||
        model == Core::StaticRef<int32>(0x1230310))
    {
        return false;
    }

    if (!Core::Hook::CallMethod<bool>(0x8AEFE0, m_vehSubObj1120, 0))
        return false;

    void* p817 = Core::Hook::Call<void*>(0x817FD0);
    bool flag = p817 && Core::Hook::CallMethod<int32>(0x920FF0, p817) > 0;

    for (int32 i = 0; i <= 3; ++i)
    {
        CPed* pPed = GetPassenger(i);
        if (pPed)
        {
            if (pPed->m_bIsInjured != 0)
                return false;
            if (*reinterpret_cast<const int32*>(static_cast<const uint8*>(pPed->m_pPedData) + 0x12C) == 2 && flag)
                return false;
            CPed* player = Core::Hook::Call<CPed*>(0x817F40);
            if (player && Core::Hook::CallMethod<CPed*>(0x9A69E0, static_cast<uint8*>(pPed->m_pPedIntelligence) + 0x2E0) == player)
                return false;
        }
    }
    return true;
}

bool __thiscall CVehicle::TryEnablePretendOccupants()
{
    if ((m_nVehFlagsF6D & 0x40) != 0)
        return true;

    uint8* p6C = reinterpret_cast<uint8*>(m_nUnk6C);
    if (p6C && (p6C[0xE] != 0 || Core::Hook::CallMethod<bool>(0x49F3F0, p6C)))
        return false;

    if (!reinterpret_cast<bool(__thiscall*)(CVehicle*)>(m_vftable[79])(this))
        return false;

    int32 tmp = -1;
    if (Core::Hook::Call<bool>(0x96A350, this, &tmp))
        return false;

    if (!HasAnyOccupants() || HasDummyOccupants())
        return false;

    if (m_pDriver && m_pDriver->m_nPedUnk219 != 0)
        return false;

    if ((m_nVehFlagsF6F & 0x20) != 0)
        return false;

    if (m_pDriver)
    {
        void* p = Core::Hook::Call<void*>(0xA26FC0, this);
        if (!p)
            return false;
        m_bHasPretendOccupants = 1;
        Core::Hook::CallMethod<void>(0xA33830, m_vehSubObjEF4, p);
    }

    auto checkPed = [](CPed* p) -> bool
    {
        return p && (Core::Hook::CallMethod<bool>(0x4443C0, p) ||
                     (p->m_nUnk6C && Core::Hook::CallMethod<bool>(0x49F3F0, reinterpret_cast<void*>(p->m_nUnk6C))));
    };

    if (checkPed(m_pDriver))
        return false;

    int32 maxPass = static_cast<int32>(m_nMaxPassengers);
    if (maxPass > 0)
    {
        for (int32 i = 0; i < maxPass; ++i)
        {
            if (checkPed(m_pPassengers[i]))
                return false;
        }
    }

    if (m_pDriver)
        Core::Hook::CallMethod<void>(0x89D8F0, m_pDriver, 0);

    if (maxPass > 0)
    {
        for (int32 i = 0; i < maxPass; ++i)
        {
            if (m_pPassengers[i])
                reinterpret_cast<void(__thiscall*)(CPed*)>((*reinterpret_cast<void***>(m_pPassengers[i]))[38])(m_pPassengers[i]);
        }
    }

    m_nVehFlagsF6D |= 0x40u;
    return true;
}

void __thiscall CVehicle::ClearOccupantsAndSetFlag400()
{
    if (HasDummyOccupants())
        return;

    if (m_pDriver)
        Core::Hook::CallMethod<void>(0x89D8F0, m_pDriver, 0);

    int32 maxPass = static_cast<int32>(m_nMaxPassengers);
    if (maxPass > 0)
    {
        for (int32 i = 0; i < maxPass; ++i)
        {
            if (m_pPassengers[i])
                reinterpret_cast<void(__thiscall*)(CPed*)>((*reinterpret_cast<void***>(m_pPassengers[i]))[38])(m_pPassengers[i]);
        }
    }

    if (m_nCreatedBy != 2)
        m_nFlags |= 0x400u;
}

bool __thiscall CVehicle::TryDisablePretendOccupants()
{
    if ((m_nVehFlagsF6D & 0x40) == 0)
        return true;

    uint8* p6C = reinterpret_cast<uint8*>(m_nUnk6C);
    if (p6C && (p6C[0xE] != 0 || Core::Hook::CallMethod<bool>(0x49F3F0, p6C)))
    {
        if (!p6C || p6C[0xE] == 0)
            Core::Hook::Call<void>(0xC28820, reinterpret_cast<void*>(Core::Rebase(0x191F104)), 2, "PRETEND_OCCUPANTS_FAIL", "PENDING_OWNER_CHANGE");
        return false;
    }

    if (Core::Hook::Call<bool>(0x462F90))
    {
        Core::StaticRef<uint8>(0xFC7EDB) = 0;
        bool ok = Core::Hook::CallMethod<bool>(0x475170, reinterpret_cast<void*>(Core::Rebase(0x18EDB70)), 1, 0);
        Core::StaticRef<uint8>(0xFC7EDB) = 1;
        if (!ok)
            return false;
    }

    if (Core::Hook::CallMethod<int32>(0x447270, Core::StaticRef<void*>(0x18A82AC)) < 5)
        return false;

    if (!Core::Hook::CallMethod<bool>(0x8F2210, reinterpret_cast<void*>(Core::Rebase(0x1354040))))
        return false;

    if (Core::Hook::Call<bool>(0x905A80, m_nModelIndex, 0x18) &&
        !Core::Hook::CallMethod<bool>(0x8F19A0, reinterpret_cast<void*>(Core::Rebase(0x1354040))))
    {
        return false;
    }

    Core::StaticRef<uint8>(0x11DC829) = 1;
    bool ok = false;
    if ((m_nVehFlagsF6F & 0x20) != 0)
    {
        Core::StaticRef<uint8>(0xFC7EDB) = 0;
        ok = Core::Hook::Call<bool>(0x83B500, this, 0, 0);
        Core::StaticRef<uint8>(0xFC7EDB) = 1;
    }
    else if (Core::Hook::Call<bool>(0x462F90))
    {
        Core::StaticRef<uint8>(0xFC7EDB) = 0;
        ok = TryAddRandomDriver(1) != nullptr;
        Core::StaticRef<uint8>(0xFC7EDB) = 1;
    }
    else
    {
        ok = Core::Hook::Call<bool>(0x83B260, this, 0, 0x63, 1);
    }

    Core::StaticRef<uint8>(0x11DC829) = 0;
    if (!ok)
        return false;

    for (int32 i = 0; i <= 3; ++i)
    {
        CPed* p = GetPassenger(i);
        if (p)
            p->m_nUnk63 = 0;
    }

    if (m_pDriver)
    {
        void* p = Core::Hook::Call<void*>(0xA26FC0, this);
        if (p && m_bHasPretendOccupants != 0)
            Core::Hook::CallMethod<void>(0xA33830, p, m_vehSubObjEF4);
    }

    m_bHasPretendOccupants = 0;
    Core::Hook::CallMethod<void>(0xA338F0, m_vehSubObjEF4);
    m_nVehFlagsF6D &= ~0x40u;
    return true;
}

void __thiscall CVehicle::CollectEntityAndOccupants(void** outArr, int32* pCount, int32 maxCount, void* pExtra)
{
    Core::Hook::CallStdcall<void>(0x9E7140, outArr, pCount, maxCount, pExtra);

    using Fn46 = void(__thiscall*)(CPed*, void**, int32*, int32, void*);
    if (m_pDriver && *pCount < maxCount)
    {
        outArr[(*pCount)++] = m_pDriver;
        reinterpret_cast<Fn46>((*reinterpret_cast<void***>(m_pDriver))[46])(m_pDriver, outArr, pCount, maxCount, nullptr);
    }

    for (int32 i = 0; i <= 3; ++i)
    {
        CPed* p = GetPassenger(i);
        if (p && p != m_pDriver && *pCount < maxCount)
        {
            outArr[(*pCount)++] = p;
            reinterpret_cast<Fn46>((*reinterpret_cast<void***>(p))[46])(p, outArr, pCount, maxCount, nullptr);
        }
    }
}

bool __thiscall CVehicle::CheckVehicleUprightAndState()
{
    const uint8* self = reinterpret_cast<const uint8*>(this);
    const uint8* p38 = static_cast<const uint8*>(m_pPhysicsInst);
    if (!p38 || *reinterpret_cast<const uint16*>(p38 + 8) == 0xFFFF || (m_nFlags & 0x1Cu) != 0)
        return true;

    bool ok = true;
    if ((m_nVehFlagsF67 & 0x40) != 0)
    {
        ok = false;
    }
    else
    {
        int32 s = *reinterpret_cast<const int32*>(self + 0x144);
        if (s == 1)
        {
            ok = false;
        }
        else if (s == 2)
        {
            if (m_pMatrix->pos.z > Core::StaticRef<float>(0xD81E78))
                ok = false;
        }
        else if (reinterpret_cast<bool(__thiscall*)(CVehicle*)>(m_vftable[85])(this) ||
                 (*reinterpret_cast<const uint32*>(self + 0x1F4) & 8u) != 0)
        {
            ok = false;
        }
    }

    int32 cntFDC = static_cast<int32>(m_nVehUnkFDC);
    for (int32 i = 0; i < cntFDC; ++i)
    {
        if ((*reinterpret_cast<const uint8*>(reinterpret_cast<const uint8*>(m_nVehUnkFD8) + i * 0x34 + 0x13) & 1u) != 0)
            ok = false;
    }

    int32 cntFD4 = static_cast<int32>(m_nVehUnkFD4);
    for (int32 i = 0; i < cntFD4; ++i)
    {
        float v = *reinterpret_cast<const float*>(reinterpret_cast<const uint8*>(m_nVehUnkFD0) + i * 0x170 + 0x160);
        if (v > Core::StaticRef<float>(0xD82150) && v < Core::StaticRef<float>(0xDAF864))
            ok = false;
    }

    if (ok && m_pDriver && m_pDriver->m_nPedUnk218 == 0 && m_pDriver->m_nPedUnk219 != 0)
    {
        void* v24 = Core::Hook::CallMethod<void*>(0x9BE5D0, m_pDriver);
        if (Core::Hook::CallMethod<int32>(0x9C5570, this) != static_cast<int32>(m_nVehUnkFD4))
        {
            if (Core::Hook::CallMethod<double>(0x839110, v24) != 0.0 ||
                Core::Hook::Call<float>(0x8347C0, Core::Hook::CallMethod<void*>(0x839330, v24)) != 0.0f)
            {
                ok = false;
            }
        }
    }

    uint8* mgr = Core::StaticRef<uint8*>(0x1666880);
    uint32 flags = *reinterpret_cast<const uint32*>(*reinterpret_cast<const uint8* const*>(mgr + 0x70) + *reinterpret_cast<const uint16*>(p38 + 8) * 8 + 4);
    if ((flags & 3u) == 1)
    {
        if (!ok)
        {
            Core::Hook::CallMethod<void>(0xA47660, this);
            return false;
        }
        return true;
    }

    void* v27 = Core::Hook::CallMethod<void*>(0x9E7080, this);
    if (v27)
    {
        void* v28 = *reinterpret_cast<void**>(reinterpret_cast<uint8*>(v27) + 0x18);
        if (v28)
        {
            uint32 val = *reinterpret_cast<const uint32*>(reinterpret_cast<const uint8*>(v28) + 0x18);
            if ((val == 0 || val == 2) && !ok)
                Core::Hook::CallMethod<void>(0x69AE50, v28, 1);
        }
    }

    return false;
}

void __thiscall CVehicle::UpdateWheelSteerOrLean(uint8* pArg, bool bFlag)
{
    float vec[4] = {};
    float* v = reinterpret_cast<float*(__thiscall*)(CVehicle*, float*)>(m_vftable[59])(this, vec);
    const CVector& up = m_pMatrix->up;
    float dot = v[2] * up.z + v[1] * up.y + v[0] * up.x;

    float f1 = Core::Hook::Call<float>(0x8348B0, pArg + 0x2918);
    float f2 = Core::Hook::Call<float>(0x8348B0, pArg + 0x2928);
    float diff = f1 - f2;

    if (bFlag)
    {
        m_fVehUnk10CC = 1.0f;
        m_fVehUnk10C8 = 0.0f;
        return;
    }

    if (((m_nVehFlagsF64 & 8) == 0 && (m_nVehFlagsF67 & 8) != 0) ||
        (m_pDriver && Core::Hook::CallMethod<bool>(0x426910, static_cast<uint8*>(m_pDriver->m_pPedIntelligence) + 0x2E0, 0x145, 0)))
    {
        m_fVehUnk10C8 = 0.0f;
        m_fVehUnk10CC = 0.0f;
        return;
    }

    float absDot = (dot >= 0.0f) ? dot : (Core::StaticRef<float>(0xEBDED0) - dot);
    float thr = Core::StaticRef<float>(0xDCC5F8);
    if (absDot < Core::StaticRef<float>(0xDCE03C))
    {
        if (f1 > thr && f2 > thr)
        {
            m_fVehUnk10C8 = f1;
            m_fVehUnk10CC = f2;
            return;
        }
        m_fVehUnk10CC = 0.0f;
        m_fVehUnk10C8 = diff;
        return;
    }

    if (f1 > thr && f2 > thr)
    {
        m_fVehUnk10C8 = f1;
        m_fVehUnk10CC = f2;
        return;
    }

    if (dot >= 0.0f)
    {
        if (diff < 0.0f)
        {
            m_fVehUnk10C8 = 0.0f;
            m_fVehUnk10CC = Core::StaticRef<float>(0xEBDED0) - diff;
            return;
        }
        m_fVehUnk10CC = 0.0f;
        m_fVehUnk10C8 = diff;
        return;
    }

    if (diff < 0.0f || (m_fVehUnk10C8 > Core::StaticRef<float>(0xDCE03C) && dot > Core::StaticRef<float>(0xEBE374)))
    {
        m_fVehUnk10CC = 0.0f;
        m_fVehUnk10C8 = diff;
        return;
    }

    m_fVehUnk10C8 = 0.0f;
    m_fVehUnk10CC = diff;
}

void CVehicle::ApplyPassengerImpulse(CPed* pPed)
{
    const uint8* p6C = reinterpret_cast<const uint8*>(m_nUnk6C);
    if (p6C && p6C[0xE] != 0)
        return;

    const uint8* pPed6C = reinterpret_cast<const uint8*>(pPed->m_nUnk6C);
    if (pPed6C && pPed6C[0xE] != 0)
        return;

    float vec1[3];
    float vec2[3];
    if (GetVehicleSubType() == 1)
    {
        float s1 = Core::StaticRef<float>(0xEBE0D8);
        vec1[0] = m_pMatrix->up.x * s1;
        vec1[1] = m_pMatrix->up.y * s1;
        vec1[2] = m_pMatrix->up.z * s1;

        void* pInfo = Core::Hook::CallMethod<void*>(0x9E86F0, pPed);
        float s2 = static_cast<float>(reinterpret_cast<double(__thiscall*)(void*)>((*reinterpret_cast<void***>(pInfo))[9])(pInfo)) * Core::StaticRef<float>(0xEBDF88);
        vec2[0] = s2 * m_pMatrix->at.x;
        vec2[1] = s2 * m_pMatrix->at.y;
        vec2[2] = s2 * m_pMatrix->at.z;
    }
    else
    {
        float dx = pPed->m_pMatrix->pos.x - m_pMatrix->pos.x;
        float dy = pPed->m_pMatrix->pos.y - m_pMatrix->pos.y;
        if (static_cast<float>(reinterpret_cast<double(__thiscall*)(CVehicle*)>(m_vftable[22])(this)) <= (dx * dx + dy * dy))
            return;

        vec2[0] = dx;
        vec2[1] = dy;
        vec2[2] = 0.0f;

        void* pInfo = Core::Hook::CallMethod<void*>(0x9E86F0, pPed);
        vec1[0] = 0.0f;
        vec1[1] = 0.0f;
        vec1[2] = static_cast<float>(reinterpret_cast<double(__thiscall*)(void*)>((*reinterpret_cast<void***>(pInfo))[9])(pInfo)) * Core::StaticRef<float>(0xEBE0E0);
    }

    Core::Hook::CallMethod<void>(0xA47B80, this, vec1, vec2, 0);
}

bool __thiscall CVehicle::AddPassenger(CPed* pPed)
{
    bool added = false;
    int32 maxPass = static_cast<int32>(m_nMaxPassengers);
    if (maxPass > 0)
    {
        for (int32 i = 0; i < maxPass; ++i)
        {
            if (!added && m_pPassengers[i] == nullptr)
            {
                m_pPassengers[i] = pPed;
                pPed->RegisterReference(reinterpret_cast<CEntity**>(&m_pPassengers[i]));
                added = true;
                break;
            }
        }
    }

    ApplyPassengerImpulse(pPed);
    return added;
}

bool __thiscall CVehicle::AddPassengerAtSeat(CPed* pPed, uint32 seatIdx)
{
    if (reinterpret_cast<bool(__thiscall*)(CVehicle*)>(m_vftable[74])(this) &&
        pPed->m_nPedUnk218 == 0 &&
        pPed->m_nPedUnk219 != 0 &&
        !reinterpret_cast<bool(__thiscall*)(CVehicle*, int32, int32, int32)>(m_vftable[77])(this, 1, 0, 0))
    {
        return false;
    }

    bool added = false;
    if (seatIdx < static_cast<uint32>(m_nMaxPassengers) && m_pPassengers[seatIdx] == nullptr)
    {
        m_pPassengers[seatIdx] = pPed;
        pPed->RegisterReference(reinterpret_cast<CEntity**>(&m_pPassengers[seatIdx]));
        added = true;
    }

    ApplyPassengerImpulse(pPed);
    return added;
}

void __thiscall CVehicle::ProcessBurningOccupants(void* a2, int32 a3)
{
    if ((m_nVehFlagsF6D & 0x40) != 0 && !reinterpret_cast<bool(__thiscall*)(CVehicle*)>(m_vftable[83])(this))
    {
        m_bHasPretendOccupants = 0;
        Core::Hook::CallMethod<void>(0xA338F0, m_vehSubObjEF4);
        m_nVehFlagsF6D &= ~0x40u;
    }

    for (int32 i = 0; i <= 3; ++i)
    {
        CPed* pPed = GetPassenger(i);
        if (!pPed)
            continue;

        uint8* p6C = reinterpret_cast<uint8*>(pPed->m_nUnk6C);
        if (p6C && p6C[0xE] != 0)
            continue;

        if (GetVehicleType() == 4 ||
            reinterpret_cast<bool(__thiscall*)(CPed*)>(pPed->m_vftable[74])(pPed) ||
            pPed->m_nDummyConvertState == 2 ||
            static_cast<float>(reinterpret_cast<double(__thiscall*)(CPed*)>(pPed->m_vftable[63])(pPed)) > Core::StaticRef<float>(0xD8D70C) ||
            Core::Hook::CallMethod<bool>(0x426910, static_cast<uint8*>(pPed->m_pPedIntelligence) + 0x2E0, 0x2E2, 0) ||
            Core::Hook::Call<double>(0x438960, 0.0f, 1.0f) >= Core::StaticRef<float>(0xD50964))
        {
            uint8 dmgInfo[0x1C] = {};
            uint8 eventBuf[0x24] = {};
            uint8 resp[0x18] = {};
            Core::Hook::CallMethod<void>(0xAB9470, dmgInfo, a2, Core::StaticRef<float>(0xD81CE0), a3, 0, 0);
            Core::Hook::CallMethod<void>(0xAB9A00, eventBuf, a2, CTimer::ms_nTimeMs, a3);
            Core::Hook::CallMethod<void>(0xABB170, dmgInfo, pPed, resp);
            if (Core::Hook::CallMethod<bool>(0x9164C0, eventBuf, pPed))
                Core::Hook::CallMethod<void>(0xAA80D0, static_cast<uint8*>(pPed->m_pPedIntelligence) + 0x84, eventBuf, 0, 1);
            Core::Hook::CallMethod<void>(0xAB9A80, eventBuf);
            Core::Hook::CallMethod<void>(0xC28820, dmgInfo);
        }
        else
        {
            uint8 fireEvt[0x58] = {};
            Core::Hook::CallMethod<void>(0x919200, fireEvt, pPed->m_pMyVehicle);
            if (Core::Hook::CallMethod<bool>(0x9164C0, fireEvt, pPed))
                Core::Hook::CallMethod<void>(0xAA80D0, static_cast<uint8*>(pPed->m_pPedIntelligence) + 0x84, fireEvt, 0, 1);
            Core::Hook::CallMethod<void>(0x919250, fireEvt);
        }
    }
}

bool __thiscall CVehicle::IsUpsideDown()
{
    return m_pMatrix->at.z <= Core::StaticRef<float>(0xEBE708);
}

bool __thiscall CVehicle::IsOnSide()
{
    float v = m_pMatrix->right.z;
    return v >= Core::StaticRef<float>(0xDCA2A8) || v <= Core::StaticRef<float>(0xEBE21C);
}

bool __thiscall CVehicle::CanPedEnterWithLockState(CPed* pPed)
{
    uint8* p6C = reinterpret_cast<uint8*>(pPed->m_nUnk6C);
    if (p6C && p6C[0xE] != 0)
        return true;

    int32 st = *reinterpret_cast<const int32*>(reinterpret_cast<const uint8*>(this) + 0x1320);
    if (Core::Hook::Call<bool>(0x462F90) && pPed->m_nPedUnk219 != 0 && st >= 9)
    {
        if (*reinterpret_cast<const int32*>(reinterpret_cast<const uint8*>(pPed->m_pPlayerInfo) + 0x574) != st - 9)
            return false;
    }

    if (st == 2 || st == 5 || st == 7 || st == 8)
        return false;

    if (st == 4)
    {
        uint8* p = Core::Hook::CallMethod<uint8*>(0x89DD80, pPed);
        if (pPed->m_nPedUnk219 == 0 && p)
        {
            CPed* pSub = Core::Hook::CallMethod<CPed*>(0xA66B40, p + 8);
            if (pSub && pSub->m_nPedUnk219 != 0)
                return true;
        }
        return false;
    }

    return pPed->m_nPedUnk219 == 0 || st != 3;
}

bool __thiscall CVehicle::IsVehicleStationaryAndLevel(bool bSkipVelCheck)
{
    if (m_nModelIndex == Core::StaticRef<int32>(0x123097C))
        return false;

    if (GetVehicleSubType() == 2)
        return true;

    float upZ = m_pMatrix->at.z;
    float vec[4] = {};
    using GetVelFn = float*(__thiscall*)(CVehicle*, float*);

    if (upZ <= Core::StaticRef<float>(0xDC9290) && upZ >= Core::StaticRef<float>(0xEBE0D8))
    {
        float* v = reinterpret_cast<GetVelFn>(m_vftable[59])(this, vec);
        float absZ = (v[2] >= 0.0f) ? v[2] : (Core::StaticRef<float>(0xEBDED0) - v[2]);
        if (absZ > Core::StaticRef<float>(0xF33A50))
            return false;

        v = reinterpret_cast<GetVelFn>(m_vftable[59])(this, vec);
        if (std::sqrtf(v[0] * v[0] + v[1] * v[1]) > Core::StaticRef<float>(0xF33A4C))
            return false;

        float* a = Core::Hook::CallMethod<float*>(0xA478E0, this, vec);
        return (a[0] * a[0] + a[1] * a[1] + a[2] * a[2]) <= Core::StaticRef<float>(0xD82320);
    }

    if (!bSkipVelCheck)
    {
        float* v = reinterpret_cast<GetVelFn>(m_vftable[59])(this, vec);
        if (std::sqrtf(v[0] * v[0] + v[1] * v[1]) > Core::StaticRef<float>(0xF33A4C))
            return false;

        v = reinterpret_cast<GetVelFn>(m_vftable[59])(this, vec);
        if (Core::Hook::Call<float>(0x424C00, v[2]) > Core::StaticRef<float>(0xF33A50))
            return false;
    }

    float* a = Core::Hook::CallMethod<float*>(0xA478E0, this, vec);
    return (a[1] * a[1] + a[2] * a[2] + a[0] * a[0]) <= Core::StaticRef<float>(0xD82320);
}

bool __thiscall CVehicle::ShouldPedBailFromSpeed(CPed* pPed)
{
    float vec[4] = {};
    float* v = reinterpret_cast<float*(__thiscall*)(CVehicle*, float*)>(m_vftable[59])(this, vec);
    float spdSq = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];

    int32 model = m_nModelIndex;
    if (model == Core::StaticRef<int32>(0x123097C))
        return false;

    float thr = (GetVehicleType() == 1)
        ? Core::StaticRef<float>(0xF33AA0)
        : Core::StaticRef<float>(0xD8F340);
    if (spdSq < thr)
        return false;

    CVehicleModelInfo* modelInfo = static_cast<CVehicleModelInfo*>(CModelInfo::ms_modelInfoPtrs[model]);
    if (*reinterpret_cast<const int32*>(reinterpret_cast<const uint8*>(modelInfo) + 0xC4) == 0x57)
        return false;

    if ((modelInfo->m_nVehicleFlags & 0x20u) != 0)
    {
        int32 seat = GetPedSeat(pPed);
        if (seat == 1 || seat == 3)
            return false;
    }

    return true;
}

bool __thiscall CVehicle::TryStartTimerF98()
{
    const uint8* p6C = reinterpret_cast<const uint8*>(m_nUnk6C);
    if (p6C && p6C[0xE] != 0)
        return false;

    if (GetVehicleType() != 0 || m_nVehUnkF98 != -1)
        return false;

    m_nVehUnkF98 = 15000;
    return true;
}

bool __cdecl CVehicle::IsSpecialModelIndex(int32 modelIdx)
{
    if (modelIdx == Core::StaticRef<int32>(0x1230898) ||
        modelIdx == Core::StaticRef<int32>(0x12309DC) ||
        modelIdx == Core::StaticRef<int32>(0x12302A4) ||
        modelIdx == Core::StaticRef<int32>(0x1230A0C) ||
        modelIdx == Core::StaticRef<int32>(0x12302F8) ||
        modelIdx == Core::StaticRef<int32>(0x12307E4) ||
        modelIdx == Core::StaticRef<int32>(0x12303A0))
    {
        return true;
    }

    return IsSpecialVehicleModel(modelIdx);
}

void __thiscall CVehicle::ResetHealthAndSubObjFD0()
{
    uint8* self = reinterpret_cast<uint8*>(this);
    if ((m_nFlags2 & 0x7C00u) != 0xC00u)
    {
        const uint8* p6C = reinterpret_cast<const uint8*>(m_nUnk6C);
        if (!p6C || p6C[0xE] == 0)
        {
            float cur = static_cast<float>(reinterpret_cast<double(__thiscall*)(CVehicle*)>(m_vftable[63])(this));
            float limit = Core::StaticRef<float>(0xD582A0);
            float target = (cur > limit) ? cur : limit;
            reinterpret_cast<void(__thiscall*)(CVehicle*, float, int32)>(m_vftable[61])(this, target, 0);
        }
    }

    Core::Hook::CallMethod<void>(0xA292B0, reinterpret_cast<void*>(Core::Rebase(0x164B030)), this);

    float f10FC = *reinterpret_cast<const float*>(self + 0x10FC);
    float f1128 = m_fVehUnk1128;
    if ((f10FC < 0.0f && f10FC > Core::StaticRef<float>(0xD6A9A4)) ||
        (f1128 > Core::StaticRef<float>(0xEBE00C) && f1128 < 0.0f))
    {
        m_fVehUnk1128 = 1.0f;
        *reinterpret_cast<float*>(self + 0x10FC) = Core::StaticRef<float>(0xD6A9A4);
    }

    int32 cnt = static_cast<int32>(m_nVehUnkFD4);
    for (int32 i = 0; i < cnt; ++i)
        Core::Hook::CallMethod<void>(0xB42E30, reinterpret_cast<uint8*>(m_nVehUnkFD0) + i * 0x170);
}

void __thiscall CVehicle::PlayPedEnterAnim(CPed* pPed, int32)
{
    bool cond = IsInActiveRange(pPed);
    if (!cond)
    {
        uint8* p6C = reinterpret_cast<uint8*>(pPed->m_nUnk6C);
        uint8* p2E0 = static_cast<uint8*>(pPed->m_pPedIntelligence) + 0x2E0;
        if (p6C && p6C[0xE] != 0 && p2E0 != nullptr)
            cond = Core::Hook::CallMethod<bool>(0x426910, p2E0, 0x2C5, 0);
    }

    if (cond)
    {
        CVehicleModelInfo* modelInfo = static_cast<CVehicleModelInfo*>(CModelInfo::ms_modelInfoPtrs[m_nModelIndex]);
        int32 id = 0x175;
        int32 v7 = Core::Hook::Call<int32>(0x8A6E40, *reinterpret_cast<const uint32*>(reinterpret_cast<const uint8*>(modelInfo) + 0xC4), &id, pPed, this, 0, 0, 0, 1);
        void* p78 = *reinterpret_cast<void**>(reinterpret_cast<uint8*>(pPed) + 0x78);
        if (Core::Hook::CallMethod<int32>(0x9EBE90, p78, v7, id) == 0)
            Core::Hook::CallMethod<void>(0x9EFEB0, p78, v7, id, Core::StaticRef<float>(0xDCB2A0), -1);
    }
}

bool __thiscall CVehicle::InitPretendOccupants()
{
    uint8* self = reinterpret_cast<uint8*>(this);
    SetEngineOrBurnoutState(false);
    m_nVehPretendOccTimeMs = CTimer::ms_nTimeMs;
    Core::Hook::CallMethod<void>(0xA338F0, m_vehSubObjEF4);
    m_nVehFlagsF6D |= 0x40u;
    self[0xF1C] = 0;
    self[0xF1B] = 10;
    self[0xF1A] = 1;
    m_bHasPretendOccupants = 1;
    return true;
}

CVehicle* __thiscall CVehicle::ScalarDeletingDtor(uint8 flags)
{
    Dtor();
    if ((flags & 1) != 0)
        Core::Hook::CallMethod<void>(0x8BE6D0, Core::StaticRef<void*>(0x1619240), this);
    return this;
}

void __thiscall CVehicle::InitFragAndPhysics()
{
    CBaseModelInfo* modelInfo = CModelInfo::ms_modelInfoPtrs[m_nModelIndex];
    if (modelInfo->m_pFragType != nullptr)
    {
        m_nFlags |= 0x4000000u;
        void* pPool = Core::Hook::CallMethod<void*>(0x96D520, Core::StaticRef<void*>(0x170665C));
        uint8* pEnt = pPool
            ? Core::Hook::CallMethod<uint8*>(0xC711F0, pPool, 0xE, modelInfo->m_pFragType, m_pMatrix, 0)
            : nullptr;
        m_pFragInstE14 = pEnt;
        Core::Hook::CallMethod<void>(0x686AD0, pEnt, 4, 1);
        *reinterpret_cast<uint32*>(pEnt + 0x60) |= 8u;
        Core::Hook::CallMethod<void>(0x9E6FA0, this, pEnt, 1);
        reinterpret_cast<void(__thiscall*)(uint8*, int32)>((*reinterpret_cast<void***>(pEnt))[40])(pEnt, 0);
        reinterpret_cast<void(__thiscall*)(uint8*)>((*reinterpret_cast<void***>(pEnt))[28])(pEnt);
    }

    uint32 flag = (Core::StaticRef<float>(0x16394E4) >= Core::StaticRef<float>(0xDCB2A8)) ? 0x100u : 0u;
    InitBrokenBoneMask(static_cast<int16>(flag));
    ApplyHiddenBoneMask();
}

void __thiscall CVehicle::CleanupPhysicsAndAudio()
{
    uint8* self = reinterpret_cast<uint8*>(this);
    uint8* p6C = reinterpret_cast<uint8*>(m_nUnk6C);
    if (p6C && p6C[0xE] != 0)
    {
        if ((m_nFlags2 & 0x7C00u) == 0xC00u)
            return;
    }
    else
    {
        reinterpret_cast<void(__thiscall*)(CVehicle*, float)>(m_vftable[60])(this, Core::StaticRef<float>(0xDAF864));
    }

    Core::Hook::CallMethod<void>(0x8B3580, m_vehSubObj1120);
    Core::Hook::CallMethod<void>(0x8AEE60, m_vehSubObj1120);
    ApplyHiddenBoneMask();
    reinterpret_cast<void(__thiscall*)(CVehicle*)>(m_vftable[72])(this);

    *reinterpret_cast<uint32*>(self + 0x1AC) = 0;
    *reinterpret_cast<uint32*>(self + 0x1B0) = 0;
    *reinterpret_cast<uint32*>(self + 0x1B4) = 0;
    *reinterpret_cast<uint32*>(self + 0x1B8) = 0;

    Core::Hook::CallMethod<void>(0x910470, reinterpret_cast<void*>(Core::Rebase(0x13DD760)), this, 0);
    Core::Hook::CallMethod<void>(0xAEAD50, m_vehSubObj260);

    uint8* p34 = static_cast<uint8*>(m_pDrawHandler);
    if (p34)
    {
        uint8* p4 = *reinterpret_cast<uint8**>(p34 + 4);
        if (p4)
            p4[0xCC] = 0;
    }

    if ((m_nFlags2 & 0x7C00u) == 0xC00u)
        *reinterpret_cast<uint32*>(self + 0x118) &= ~0x10000u;
}

void __thiscall CVehicle::SetHeading(void* a2, float angle, int32)
{
    if (angle >= Core::StaticRef<float>(0xEBDFFC) && angle <= Core::StaticRef<float>(0xEBDEE4))
    {
        if (m_pMatrix)
        {
            float c = std::cosf(angle);
            float s = std::sinf(angle);
            m_pMatrix->right = { c, s, 0.0f };
            m_pMatrix->up = { -s, c, 0.0f };
            m_pMatrix->at = { 0.0f, 0.0f, 1.0f };
        }
        else
        {
            m_fPad1C = angle;
        }
    }

    reinterpret_cast<void(__thiscall*)(CVehicle*, void*, int32, int32)>(m_vftable[2])(this, a2, 1, 0);
    Core::Hook::CallMethod<void>(0x9E1F70, this);
    reinterpret_cast<void(__thiscall*)(CVehicle*, int32)>(m_vftable[45])(this, 1);
    reinterpret_cast<void(__thiscall*)(CVehicle*)>(m_vftable[100])(this);
    Core::Hook::CallMethod<void>(0xC7E570, m_vehSubObj10E0);
    Core::Hook::CallMethod<void>(0x8AEE60, m_vehSubObj1120);

    uint8* pSub = Core::Hook::CallMethod<uint8*>(0x9E7080, this);
    if (pSub)
    {
        reinterpret_cast<void(__thiscall*)(uint8*)>((*reinterpret_cast<void***>(pSub))[4])(pSub);
        void* p18 = *reinterpret_cast<void**>(pSub + 0x18);
        if (p18)
            Core::Hook::CallMethod<void>(0x69AE50, p18, 1);
    }

    *reinterpret_cast<uint32*>(reinterpret_cast<uint8*>(this) + 0x118) &= ~0x80000u;
    UpdateNetworkOrPhysicsActive(true);
    Core::Hook::CallMethod<void>(0x7C8C00, reinterpret_cast<void*>(Core::Rebase(0x110A890)), this);
}

bool __cdecl CVehicle::ArePedsAwareOrHostile(CPed* pPedA, CPed* pPedB)
{
    return Core::Hook::CallMethod<bool>(0x9C4EF0, pPedA->m_pPedIntelligence, pPedB) ||
           Core::Hook::CallMethod<bool>(0x9C4EF0, pPedB->m_pPedIntelligence, pPedA);
}

bool __thiscall CVehicleSubObj5160::IsInNegativeRange()
{
    float val = *reinterpret_cast<const float*>(reinterpret_cast<const uint8*>(this) + 8);
    return val > Core::StaticRef<float>(0xEBE00C) && val < 0.0f;
}

CVehicleSubObj5180* __thiscall CVehicleSubObj5180::Ctor()
{
    uint32* p = reinterpret_cast<uint32*>(this);
    for (int32 i = 0; i < 8; ++i)
        p[i] = 0;
    return this;
}

CVehicleLink* __thiscall CVehicleLink::Ctor()
{
    m_pOwner = nullptr;
    m_pAttached = nullptr;
    m_pPending = nullptr;
    m_nBoneId = 1;
    m_bFlag = 0;
    m_nTimer = 0;
    return this;
}

void __thiscall CVehicleLink::Dtor()
{
    if (m_pAttached)
    {
        static_cast<CEntity*>(m_pAttached)->CleanUpOldReference(reinterpret_cast<CEntity**>(&m_pAttached));
        m_pAttached = nullptr;
    }
    if (m_pPending)
    {
        static_cast<CEntity*>(m_pPending)->CleanUpOldReference(reinterpret_cast<CEntity**>(&m_pPending));
        m_pPending = nullptr;
    }
}

void __thiscall CVehicleLink::SetBoneId(int32 boneId)
{
    m_nBoneId = boneId;
}

void __thiscall CVehicleLink::SetAttached(CEntity* pEntity)
{
    if (m_pAttached)
        static_cast<CEntity*>(m_pAttached)->CleanUpOldReference(reinterpret_cast<CEntity**>(&m_pAttached));
    m_pAttached = pEntity;
    if (pEntity)
        pEntity->RegisterReference(reinterpret_cast<CEntity**>(&m_pAttached));
}

bool __thiscall CVehicleLink::IsAttached(const CEntity* pEntity)
{
    return m_pAttached == pEntity;
}

void __thiscall CVehicleLink::SetPending(CEntity* pEntity)
{
    if (m_pPending)
    {
        static_cast<CEntity*>(m_pPending)->CleanUpOldReference(reinterpret_cast<CEntity**>(&m_pPending));
        m_pPending = nullptr;
    }
    m_pPending = pEntity;
    pEntity->RegisterReference(reinterpret_cast<CEntity**>(&m_pPending));
}

bool __thiscall CVehicleLink::HasAnyEntity()
{
    return m_pAttached != nullptr || m_pPending != nullptr;
}

bool __thiscall CVehicleLink::IsValidBoneId()
{
    return m_nBoneId > 0 && m_nBoneId < 5;
}

void __thiscall CVehicle::ResetDrawHandler()
{
    uint8* p34 = static_cast<uint8*>(m_pDrawHandler);
    uint8* p4 = p34 ? *reinterpret_cast<uint8**>(p34 + 4) : nullptr;
    if (p4 != nullptr)
        Core::Hook::CallMethod<void>(0xCBA620, p4, this, -1);
}

int32 __thiscall CVehicle::GetVehicleClassFromFlags()
{
    uint32 flags = *reinterpret_cast<const uint32*>(*reinterpret_cast<const uint8* const*>(reinterpret_cast<const uint8*>(this) + 0xE18) + 0xEC) & 0xF000000u;
    switch (flags)
    {
    case 0x0000000u: return 1;
    case 0x1000000u: return 2;
    case 0x2000000u: return 3;
    case 0x4000000u: return 5;
    case 0x8000000u: return 4;
    default:         return 0;
    }
}

void __thiscall CVehicle::RemoveDriver(bool bSetFlag)
{
    if (m_pDriver != nullptr)
    {
        if ((m_nFlags2 & 0x7C00u) != 0xC00u && bSetFlag)
            m_nFlags2 = (m_nFlags2 & 0xFFFF8BFFu) | 0x800u;
        m_pDriver->CleanUpOldReference(reinterpret_cast<CEntity**>(&m_pDriver));
        m_pDriver = nullptr;
    }
}

CPed* __thiscall CVehicle::TryAddRandomDriver(int32 arg)
{
    if (m_pDriver != nullptr)
        return m_pDriver;

    CPed* pPed = Core::Hook::Call<CPed*>(0x9FBC30, this, 1, 0, 1, arg, 0);
    if (!pPed)
        return nullptr;

    if ((m_nVehFlagsF70 & 0x10u) != 0 && Core::Hook::CallMethod<bool>(0x89DE90, pPed))
        Core::Hook::Call<void>(0x9F8B20, pPed, 1, 0, 0);

    pPed->m_nUnk63 = m_nUnk63;
    return m_pDriver;
}

CPed* __thiscall CVehicle::TryAddRandomPassenger(int32 seatIdx)
{
    if (m_pPassengers[seatIdx] != nullptr)
        return m_pPassengers[seatIdx];

    CPed* pPed = Core::Hook::Call<CPed*>(0x9FBC30, this, 0, seatIdx, 1, 1, 0);
    if (!pPed)
        return nullptr;

    if ((m_nVehFlagsF70 & 0x10u) != 0 && Core::Hook::CallMethod<bool>(0x89DE90, pPed))
        Core::Hook::Call<void>(0x9F8B20, pPed, 1, 0, 0);

    pPed->m_nUnk63 = m_nUnk63;
    int32 pedModel = pPed->m_nModelIndex;
    uint32 pedType = *reinterpret_cast<const uint32*>(static_cast<const uint8*>(pPed->m_pPedData) + 0x12C);
    if (pedType == 0x10u || pedType == 0x11u || pedType == 2u || (pedType - 3u) <= 0xBu || pedType == 0x12u)
        return pPed;

    auto rejectPed = [this, pPed]()
    {
        RemovePassenger(pPed);
        if (pPed->m_pMyVehicle)
        {
            pPed->m_pMyVehicle->CleanUpOldReference(reinterpret_cast<CEntity**>(&pPed->m_pMyVehicle));
            pPed->m_pMyVehicle = nullptr;
        }
        pPed->m_nPedFlags2 &= ~4u;
        Core::Hook::Call<void>(0x9F8D40, pPed, 1);
    };

    if (m_pDriver && m_pDriver->m_nModelIndex == pedModel)
    {
        rejectPed();
        return nullptr;
    }

    if (seatIdx > 0)
    {
        for (int32 i = 0; i < seatIdx; ++i)
        {
            CPed* other = m_pPassengers[i];
            if (other && pPed && other != pPed && other->m_nModelIndex == pedModel)
            {
                rejectPed();
                pPed = nullptr;
            }
        }
    }

    return pPed;
}

bool __thiscall CVehicle::IsPedPassenger(const CPed* pPed)
{
    if (!pPed)
        return false;

    for (int32 i = 0; i < 8; ++i)
    {
        if (m_pPassengers[i] == pPed)
            return true;
    }
    return false;
}

bool __thiscall CVehicle::IsPedDriver(const CPed* pPed)
{
    return pPed != nullptr && m_pDriver == pPed;
}

bool __thiscall CVehicle::IsDoorLockStateUnlocked()
{
    uint32 st = static_cast<uint32>(m_nVehDoorLockState1320);
    return st == 0 || st == 1 || st == 7;
}

void __thiscall CVehicle::ProcessAlarm()
{
    uint16 timer = static_cast<uint16>(m_nVehUnkF98);
    if (timer != 0 && timer != 0xFFFF)
    {
        uint32 dec = static_cast<uint32>(static_cast<int64>(CTimer::ms_fFrameTimeStep * Core::StaticRef<float>(0xDAF864)));
        if (static_cast<uint32>(timer) >= dec)
        {
            m_nVehUnkF98 = static_cast<int16>(timer - Core::Hook::Call<int32>(0x448330));
        }
        else
        {
            m_nVehUnkF98 = 0;
            m_nVehUnk133C = 0;
        }
    }
}

bool __thiscall CVehicle::IsPointInBoundingBox(float x, float y, float z, float margin)
{
    CVector d(x - m_pMatrix->pos.x, y - m_pMatrix->pos.y, z - m_pMatrix->pos.z);
    CBaseModelInfo* mi = CModelInfo::ms_modelInfoPtrs[m_nModelIndex];

    float lx = m_pMatrix->right.x * d.x + m_pMatrix->right.y * d.y + m_pMatrix->right.z * d.z;
    if (lx < mi->m_vBoundCentre.x - margin || lx > mi->m_vMaxBounds.x + margin)
        return false;

    float ly = m_pMatrix->up.x * d.x + m_pMatrix->up.y * d.y + m_pMatrix->up.z * d.z;
    if (ly < mi->m_vBoundCentre.y - margin || ly > mi->m_vMaxBounds.y + margin)
        return false;

    float lz = m_pMatrix->at.x * d.x + m_pMatrix->at.y * d.y + m_pMatrix->at.z * d.z;
    return lz >= mi->m_vBoundCentre.z - margin && lz <= mi->m_vMaxBounds.z + margin;
}

int32 __cdecl CVehicle::GetVehicleAnimGroupIndex(int32 animGroupId)
{
    void* mgr = Core::StaticRef<void*>(0x15F5514);
    return (*reinterpret_cast<int32(__thiscall***)(void*, int32)>(mgr))[2](mgr, animGroupId);
}

bool __cdecl CVehicle::IsSpecialVehicleModel(int32 modelIdx)
{
    return modelIdx == Core::StaticRef<int32>(0x1230424) ||
           modelIdx == Core::StaticRef<int32>(0x1230460) ||
           modelIdx == Core::StaticRef<int32>(0x12305EC) ||
           modelIdx == Core::StaticRef<int32>(0x123076C) ||
           modelIdx == Core::StaticRef<int32>(0x12304D8) ||
           modelIdx == Core::StaticRef<int32>(0x1230784) ||
           modelIdx == Core::StaticRef<int32>(0x1230904) ||
           modelIdx == Core::StaticRef<int32>(0x12304B4) ||
           modelIdx == Core::StaticRef<int32>(0x123022C) ||
           modelIdx == Core::StaticRef<int32>(0x1230A9C) ||
           modelIdx == Core::StaticRef<int32>(0x1230394) ||
           modelIdx == Core::StaticRef<int32>(0x12306F4) ||
           modelIdx == Core::StaticRef<int32>(0x1230814);
}

bool __thiscall CVehicle::IsConvertibleRoofClosedOrValid()
{
    CVehicleModelInfo* mi = static_cast<CVehicleModelInfo*>(CModelInfo::ms_modelInfoPtrs[m_nModelIndex]);
    if ((mi->m_nVehicleFlags & 0x20000u) == 0)
        return true;

    if (GetVehicleType() != 0)
        return false;

    const int32* pCC = reinterpret_cast<const int32*>(mi->m_pVehicleStruct);
    if ((pCC[0x168 / 4] == -1 || (m_nVehBoneMaskE20 & 4u) != 0) &&
        (pCC[0x16C / 4] == -1 || (m_nVehBoneMaskE20 & 8u) != 0) &&
        (pCC[0x170 / 4] == -1 || (m_nVehBoneMaskE20 & 0x10u) != 0))
    {
        return false;
    }

    return true;
}

bool __thiscall CVehicle::SetDriver(CPed* pPed, uint32 arg4)
{
    if (reinterpret_cast<bool(__thiscall*)(CVehicle*)>(m_vftable[74])(this) &&
        pPed->m_nPedUnk218 == 0 &&
        pPed->m_nPedUnk219 != 0 &&
        !reinterpret_cast<bool(__thiscall*)(CVehicle*, int32, int32, CPed*)>(m_vftable[77])(this, 1, (static_cast<uint8>(arg4) == 0) ? 1 : 0, pPed))
    {
        return false;
    }

    if (m_nUnk6C && pPed->m_nUnk6C && pPed->m_nPedUnk219 == 0)
    {
        if (Core::Hook::CallMethod<uint8>(0x49F380, reinterpret_cast<void*>(m_nUnk6C)) !=
            Core::Hook::CallMethod<uint8>(0x49F380, reinterpret_cast<void*>(pPed->m_nUnk6C)))
        {
            Core::Hook::CallStdcall<void>(0x92C4D0, 0, 0);
            Core::Hook::CallMethod<uint32>(0x49F460, reinterpret_cast<void*>(m_nUnk6C));
            Core::Hook::CallMethod<uint32>(0x49F460, reinterpret_cast<void*>(pPed->m_nUnk6C));
            Core::Hook::Call<void>(0xC28820, 0, 0, 0, 0, 0, 0, 0, 0, 0);
            Core::Hook::CallStdcall<void>(0x92C4D0, 0, 0);
        }
    }

    if (m_pDriver != nullptr)
    {
        if (m_pDriver != pPed)
            return false;
        m_pDriver->CleanUpOldReference(reinterpret_cast<CEntity**>(&m_pDriver));
    }

    m_pDriver = pPed;
    pPed->RegisterReference(reinterpret_cast<CEntity**>(&m_pDriver));

    if ((m_nVehFlagsF65 & 2u) != 0 && pPed->m_nPedUnk219 != 0)
    {
        uint8* pedBytes = reinterpret_cast<uint8*>(pPed);
        if (GetVehicleType() == 4 && Core::StaticRef<uint8>(0x1549119) != 0)
        {
            if (*reinterpret_cast<int32*>(pedBytes + 0x340) == 0)
            {
                Core::Hook::CallMethod<void>(0x9AB750, pedBytes + 0x2B0, 0x12, 8, 0, 1, 0);
            }
            else
            {
                int32 cur = Core::Hook::CallMethod<int32>(0x447370, pedBytes + 0x344);
                int16 rem = static_cast<int16>(8 - cur);
                int32 ammo = (rem >= 0) ? static_cast<uint16>(rem) : 0;
                Core::Hook::CallMethod<void>(0x9AA280, pedBytes + 0x2B0, *reinterpret_cast<int32*>(pedBytes + 0x340), ammo);
            }
        }
        else
        {
            if ((m_nVehFlagsF6F & 0x40u) != 0)
            {
                if (Core::StaticRef<uint8>(0xF2A059) != 0)
                {
                    int32 w = *reinterpret_cast<int32*>(pedBytes + 0x310);
                    if (w == 0)
                        Core::Hook::CallMethod<void>(0x9AB750, pedBytes + 0x2B0, 0xB, 5, 0, 1, 0);
                    else
                        Core::Hook::CallMethod<void>(0x9AA280, pedBytes + 0x2B0, w, 5);
                }
                if (m_nModelIndex == Core::StaticRef<int32>(0x1230904))
                {
                    float h = static_cast<float>(Core::Hook::CallMethod<double>(0x447640, pedBytes + 0xB94)) + Core::StaticRef<float>(0xD5A154);
                    float maxH = static_cast<float>(*reinterpret_cast<const uint16*>(reinterpret_cast<const uint8*>(pPed->m_pPlayerInfo) + 0x54C));
                    Core::Hook::CallMethod<void>(0x89E640, pPed, (maxH > h) ? h : maxH);
                }
            }
            else if (m_nModelIndex == Core::StaticRef<int32>(0x1230778))
            {
                uint16 pid = static_cast<uint16>(Core::Hook::Call<int32>(0x817DC0, pPed));
                float argF;
                std::memcpy(&argF, &arg4, sizeof(argF));
                float curA = static_cast<float>(reinterpret_cast<double(__thiscall*)(CPed*)>(pPed->m_vftable[63])(pPed)) + Core::StaticRef<float>(0xDCEA38);
                float newA = (curA > argF)
                    ? static_cast<float>(*reinterpret_cast<const uint16*>(Core::Hook::Call<uint8*>(0x817F20, static_cast<uint32>(pid)) + 0x54A))
                    : (static_cast<float>(reinterpret_cast<double(__thiscall*)(CPed*)>(pPed->m_vftable[63])(pPed)) + Core::StaticRef<float>(0xDCEA38));
                reinterpret_cast<void(__thiscall*)(CPed*, float, int32)>(pPed->m_vftable[61])(pPed, newA, 1);
            }
            m_nVehFlagsF65 &= ~2u;
        }
    }

    if (GetVehicleSubType() == 1)
        Core::Hook::CallMethod<void>(0xC03730, this, 0, 1);

    return true;
}

void __thiscall CVehicle::UpdateUpsideDownAlarmOrFireFlag()
{
    if ((m_nVehFlagsF6D & 2u) != 0)
        return;

    if (((static_cast<uint8>(m_nRandomSeed) + Core::StaticRef<uint32>(0x11DDF38)) & 0x1Fu) != 0)
        return;

    uint32 t = m_nVehUnk1300;
    if (t == 0 || t + 0x36B0u > CTimer::ms_nTimeMs)
        return;

    if (GetVehicleType() != 0 || m_nCreatedBy == 2)
        return;

    if (m_pMatrix->at.z < Core::StaticRef<float>(0xD503BC))
        return;

    if ((*reinterpret_cast<const uint8*>(reinterpret_cast<const uint8*>(this) + 0x11A) & 1u) != 0)
        return;

    if (HasAnyOccupants())
        return;

    if (Core::Hook::Call<bool>(0x8FF630, &m_pMatrix->pos))
        return;

    m_nVehFlagsF6D |= 2u;
}

bool __thiscall CVehicle::IsOnFireFlagActive()
{
    return (m_nVehFlagsF6D & 2u) != 0 &&
           Core::Hook::CallMethod<double>(0x8AE610, m_vehSubObj1120) > 0.0 &&
           m_fVehUnk1128 > Core::StaticRef<float>(0xDECD60);
}

double __stdcall CVehicle::GetExcessSpeedOrInvalid(float speed, int32)
{
    float thr = (Core::StaticRef<int32>(0x10F8044) < 2)
        ? Core::StaticRef<float>(0xD60ABC)
        : Core::StaticRef<float>(0xD82158);
    return (speed >= thr)
        ? static_cast<double>(speed - thr)
        : static_cast<double>(Core::StaticRef<float>(0xEBDED4));
}

CPed* __thiscall CVehicle::GetRandomPassenger()
{
    int32 start = static_cast<int32>(static_cast<float>(Core::Hook::Call<int32>(0xD0D21A) & 0xFFFF) *
                                     Core::StaticRef<float>(0xEBE1D8) *
                                     Core::StaticRef<float>(0xD5829C));
    for (int32 i = 0; i < 8; ++i)
    {
        int32 idx = (i + start) % 8;
        if (m_pPassengers[idx] != nullptr)
            return m_pPassengers[idx];
    }
    return nullptr;
}

bool __thiscall CVehicle::HasAnyActiveOccupant219()
{
    if (m_pDriver && m_pDriver->m_nPedUnk219 != 0)
        return true;

    for (int32 i = 0; i < 8; ++i)
    {
        if (m_pPassengers[i] && m_pPassengers[i]->m_nPedUnk219 != 0)
            return true;
    }
    return false;
}

bool __thiscall CVehicle::HasAliveOccupants()
{
    if (m_pDriver && m_pDriver->m_nPedUnk218 == 0 && m_pDriver->m_nPedUnk219 != 0)
        return true;

    for (int32 i = 0; i < 8; ++i)
    {
        if (m_pPassengers[i] && m_pPassengers[i]->m_nPedUnk218 == 0 && m_pPassengers[i]->m_nPedUnk219 != 0)
            return true;
    }
    return false;
}

bool __thiscall CVehicle::IsPedOccupant(const CPed* pPed)
{
    if (m_pDriver == pPed)
        return true;

    for (int32 i = 0; i < 8; ++i)
    {
        if (m_pPassengers[i] == pPed)
            return true;
    }
    return false;
}

bool __thiscall CVehicle::IsInActiveRange(void*)
{
    if ((m_nFlags & 0x40000u) == 0)
        return true;

    if (Core::StaticRef<uint8>(0xF0C511) == 0 || Core::StaticRef<uint8>(0x10D0189) != 0)
        return false;

    const CVector& pos = m_pMatrix->pos;
    float dx = pos.x - Core::StaticRef<float>(0x11DC4E0);
    float dy = pos.y - Core::StaticRef<float>(0x11DC4E4);
    float dz = pos.z - Core::StaticRef<float>(0x11DC4E8);
    float distSq = dx * dx + dy * dy + dz * dz;

    CPed* pPlayer = Core::Hook::Call<CPed*>(0x817F40);
    if (pPlayer)
    {
        const CVector& pPos = pPlayer->m_pMatrix->pos;
        dx = pos.x - pPos.x;
        dy = pos.y - pPos.y;
        dz = pos.z - pPos.z;
        distSq = dx * dx + dy * dy + dz * dz;
    }

    return Core::Hook::Call<double>(0x799240) > distSq;
}

bool __thiscall CVehicle::CanBeEnteredByPed()
{
    if ((m_nVehFlagsF68 & 0x10u) != 0)
        return false;

    CVehicleModelInfo* mi = static_cast<CVehicleModelInfo*>(CModelInfo::ms_modelInfoPtrs[m_nModelIndex]);
    return static_cast<int32>(mi->m_pVehicleStruct->m_nBones[1]) != -1;
}

void __thiscall CVehicle::DecrementUnk133C()
{
    if (m_nVehUnk133C > 0)
        --m_nVehUnk133C;
}

bool __thiscall CVehicle::ShouldSkipLodOrUpdate()
{
    int32 v1 = (Core::StaticRef<int32>(0x11D5308) != -1)
        ? Core::StaticRef<int32>(0x11D5308)
        : Core::StaticRef<int32>(0x11D5300);
    if (v1 > 20)
        return true;

    int32 rand6 = static_cast<int32>(m_nRandomSeed & 0x3Fu);
    int32 v5 = (Core::StaticRef<int32>(0x11D52F0) != -1)
        ? Core::StaticRef<int32>(0x11D52F0)
        : Core::StaticRef<int32>(0x11D52FC);
    if (v1 > 19 && v5 > rand6)
        return true;
    if (v1 < 6)
        return true;
    if (v1 < 7 && v5 < rand6)
        return true;

    float fVal = static_cast<float>(m_nRandomSeed) * Core::StaticRef<float>(0xD8215C);
    if (Core::StaticRef<float>(0x1639534) > fVal || Core::StaticRef<float>(0x16394E4) > fVal)
        return true;

    int32 vt = GetVehicleType();
    if (vt < 0)
        return false;
    if (vt > 1)
        return (vt == 3) && (m_nFlags & 0x8000000u) != 0;

    uint32 slot = *reinterpret_cast<const uint32*>(reinterpret_cast<const uint8*>(this) + 0xE2C + ((m_nRandomSeed & 3u) * 4));
    uint16 idx = static_cast<uint16>(slot & 0xFFFFu);
    if (idx != 0xFFFFu)
    {
        uint8* entry = reinterpret_cast<uint8**>(Core::Rebase(0x1640B64))[idx];
        if (entry && (entry[(slot >> 16) * 32 + 0x1D] & 0x80u) != 0)
            return true;
    }

    return false;
}

void __thiscall CVehicle::ResetBoneChild(int32 boneSlot)
{
    int32 t = GetVehicleType();
    if (t != 0 && t != 2 && t != 1)
        return;

    void* pFrag = m_pFragInstE14 ? m_pFragInstE14 : m_pPhysicsInst;
    if (!pFrag)
        return;

    uint8* v5 = Core::Hook::CallMethod<uint8*>(0x51DDD0, pFrag);
    CVehicleModelInfo* mi = static_cast<CVehicleModelInfo*>(CModelInfo::ms_modelInfoPtrs[m_nModelIndex]);
    int32 target = static_cast<int32>(mi->m_pVehicleStruct->m_nBones[boneSlot]);
    if (target == -1)
        return;

    int32 count = static_cast<int32>(v5[0x1F3]);
    void** arr = *reinterpret_cast<void***>(v5 + 0xD4);
    for (int32 i = 0; i < count; ++i)
    {
        if (*reinterpret_cast<const int16*>(static_cast<const uint8*>(arr[i]) + 0xE) == static_cast<int16>(target))
            Core::Hook::CallMethod<void>(0x518180, pFrag, i);
    }
}

void __thiscall CVehicle::SetBoneHidden(int32 boneSlot, bool bHide)
{
    if (static_cast<uint32>(boneSlot - 89) > 9u)
        return;

    CVehicleModelInfo* mi = static_cast<CVehicleModelInfo*>(CModelInfo::ms_modelInfoPtrs[m_nModelIndex]);
    int32 target = static_cast<int32>(mi->m_pVehicleStruct->m_nBones[boneSlot]);
    if (target == -1)
        return;

    uint32 bit = 1u << (static_cast<uint8>(boneSlot) - 88);
    if (bHide)
    {
        m_nVehBoneMaskE20 |= bit;
        if (m_pFragInstE14)
        {
            uint8* v5 = Core::Hook::CallMethod<uint8*>(0x51DDD0, m_pFragInstE14);
            int32 count = static_cast<int32>(v5[0x1F3]);
            void** arr = *reinterpret_cast<void***>(v5 + 0xD4);
            for (int32 i = 0; i < count; ++i)
            {
                if (*reinterpret_cast<const int16*>(static_cast<const uint8*>(arr[i]) + 0xE) == static_cast<int16>(target))
                    Core::Hook::CallMethod<void>(0x518180, m_pFragInstE14, i);
            }
        }
    }
    else
    {
        m_nVehBoneMaskE20 &= ~bit;
        reinterpret_cast<void(__thiscall*)(CVehicle*)>(m_vftable[101])(this);
    }
}

bool __thiscall CVehicle::IsBoneVisible(int32 boneSlot)
{
    CVehicleModelInfo* mi = static_cast<CVehicleModelInfo*>(CModelInfo::ms_modelInfoPtrs[m_nModelIndex]);
    int32 target = static_cast<int32>(mi->m_pVehicleStruct->m_nBones[boneSlot]);
    return target != -1 && (m_nVehBoneMaskE20 & (1u << (static_cast<uint8>(boneSlot) - 88))) == 0;
}

void __thiscall CVehicle::SetCreatedBy(uint8 createdBy)
{
    if (m_nCreatedBy == createdBy)
        return;

    Core::Hook::Call<void>(0x83B3E0, this, 1);
    m_nCreatedBy = createdBy;
    Core::Hook::Call<void>(0x83B3E0, this, 0);

    if (m_nCreatedBy == 2)
    {
        void* p80 = reinterpret_cast<uint8*>(this) + 0x80;
        Core::Hook::CallMethod<void>(0xB2A8F0, p80, true);
        Core::Hook::Call<void>(0xB29E50, p80);
    }
}

void __thiscall CVehicle::ClearUnkFlagsF64_1344()
{
    m_nVehFlagsF64 &= 0xE7u;
    m_nVehUnk1344 = 0;
}

double __thiscall CVehicle::GetBurnoutOrEngineHealthFactor()
{
    if ((m_nFlags2 & 0x7C00u) == 0xC00u ||
        *reinterpret_cast<const float*>(reinterpret_cast<const uint8*>(this) + 0x10FC) <= 0.0f)
    {
        return 1.0;
    }

    if (m_nVehUnk1344 >= 3 ||
        Core::StaticRef<uint8>(0x1670543) != 0 ||
        Core::Hook::Call<bool>(0x462F90) ||
        m_nCreatedBy == 2)
    {
        return 0.0;
    }

    float mult = 1.0f;
    if (m_pDriver)
    {
        uint8* pIntel = static_cast<uint8*>(m_pDriver->m_pPedIntelligence);
        if (pIntel)
        {
            void* pTask = Core::Hook::CallMethod<void*>(0x426850, pIntel + 0x44, 0x2C5);
            if (pTask && Core::Hook::CallMethod<bool>(0x9B7690, pTask))
            {
                if (Core::Hook::CallMethod<bool>(0xAEABE0, m_vehSubObj260))
                {
                    mult = 0.4f;
                }
                else
                {
                    switch (5 - *reinterpret_cast<const int32*>(reinterpret_cast<const uint8*>(this) + 0xFC8))
                    {
                    case 1: mult = 0.9f; break;
                    case 2: mult = 0.8f; break;
                    case 3: mult = 0.7f; break;
                    case 4: mult = 0.6f; break;
                    case 5: mult = 0.5f; break;
                    default: mult = 1.0f; break;
                    }
                }
            }
        }
    }

    double d = reinterpret_cast<double(__thiscall*)(CVehicle*)>(m_vftable[63])(this);
    return 1.0 - d * Core::StaticRef<float>(0xDCB2AC) * mult;
}

bool __thiscall CVehicle::HasAnyWheelUnk160Not1000()
{
    int32 count = static_cast<int32>(m_nVehUnkFD4);
    float limit = Core::StaticRef<float>(0xDAF864);
    for (int32 i = 0; i < count; ++i)
    {
        if (*reinterpret_cast<const float*>(reinterpret_cast<const uint8*>(m_nVehUnkFD0) + i * 0x170 + 0x160) != limit)
            return true;
    }
    return false;
}

bool __thiscall CVehicle::HasAnyWheelUnk15CNot1000()
{
    int32 count = static_cast<int32>(m_nVehUnkFD4);
    float limit = Core::StaticRef<float>(0xDAF864);
    for (int32 i = 0; i < count; ++i)
    {
        if (*reinterpret_cast<const float*>(reinterpret_cast<const uint8*>(m_nVehUnkFD0) + i * 0x170 + 0x15C) != limit)
            return true;
    }
    return false;
}

int32 __thiscall CVehicle::GetModelInfoEntryIndex(void* pEntry)
{
    uint8* mi = reinterpret_cast<uint8*>(CModelInfo::ms_modelInfoPtrs[m_nModelIndex]);
    int32 count = *reinterpret_cast<const int32*>(mi + 0x3BC);
    for (int32 i = 0; i < count; ++i)
    {
        if (mi + 0x350 + i * 12 == pEntry)
            return i;
    }
    return -1;
}

void __thiscall CVehicle::ResolveContactPenetration(void* pContacts)
{
    struct Contact
    {
        int32   active;
        uint8   pad04[0x0C];
        CVector pos;
        float   pad1C;
        CVector normal;
        float   pad2C;
        uint8   pad30[0x30];
    };

    const Contact* contacts = static_cast<const Contact*>(pContacts);
    int32 count = static_cast<int32>(m_nVehUnkFD4);
    float sumX = 0.0f;
    float sumY = 0.0f;
    float sumZ = 0.0f;
    int32 nActive = 0;

    for (int32 i = 0; i < count; ++i)
    {
        if (contacts[i].active != 0)
        {
            sumX += contacts[i].normal.x;
            sumY += contacts[i].normal.y;
            sumZ += contacts[i].normal.z;
            ++nActive;
        }
    }

    if (nActive == 0)
        return;

    double invN = 1.0 / static_cast<double>(nActive);
    float avgX = static_cast<float>(sumX * invN);
    float avgY = static_cast<float>(sumY * invN);
    float avgZ = static_cast<float>(sumZ * invN);

    float maxProj = Core::StaticRef<float>(0xEBE02C);
    for (int32 i = 0; i < count; ++i)
    {
        if (contacts[i].active != 0)
        {
            float proj = contacts[i].pos.x * avgX + contacts[i].pos.y * avgY + contacts[i].pos.z * avgZ;
            if (proj > maxProj)
                maxProj = proj;
        }
    }

    const float* pMin = reinterpret_cast<const float*(__thiscall*)(CVehicle*)>(m_vftable[24])(this);
    const float* pMax = reinterpret_cast<const float*(__thiscall*)(CVehicle*)>(m_vftable[25])(this);

    CVector world[8];
    for (int32 i = 0; i < 8; ++i)
    {
        CVector local(
            (i & 4) ? pMin[0] : pMax[0],
            (i & 2) ? pMin[1] : pMax[1],
            (i & 1) ? pMin[2] : pMax[2]);

        if (m_pMatrix)
        {
            world[i].x = m_pMatrix->right.x * local.x + m_pMatrix->up.x * local.y + m_pMatrix->at.x * local.z + m_pMatrix->pos.x;
            world[i].y = m_pMatrix->right.y * local.x + m_pMatrix->up.y * local.y + m_pMatrix->at.y * local.z + m_pMatrix->pos.y;
            world[i].z = m_pMatrix->right.z * local.x + m_pMatrix->up.z * local.y + m_pMatrix->at.z * local.z + m_pMatrix->pos.z;
        }
        else
        {
            Core::Hook::Call<void>(0x9E77F0, &world[i], &m_vecPos, &local);
        }
    }

    float minDist = Core::StaticRef<float>(0xEBE030);
    for (int32 i = 0; i < 8; ++i)
    {
        float d = (world[i].x - avgX * maxProj) * avgX +
                  (world[i].y - avgY * maxProj) * avgY +
                  (world[i].z - avgZ * maxProj) * avgZ;
        if (d < minDist)
            minDist = d;
    }

    if (minDist < 0.0f)
    {
        float push = Core::StaticRef<float>(0xDCB2AC) - minDist;
        CMatrix mat = *m_pMatrix;
        mat.pos.x += avgX * push;
        mat.pos.y += avgY * push;
        mat.pos.z += avgZ * push;

        for (int32 i = 0; i < count; ++i)
            Core::Hook::CallMethod<void>(0xB47B60, reinterpret_cast<uint8*>(m_nVehUnkFD0) + i * 0x170, m_pMatrix, &mat);

        reinterpret_cast<void(__thiscall*)(CVehicle*, CMatrix*, int32, int32)>(m_vftable[1])(this, &mat, 0, 0);
    }
}

void __thiscall CVehicle::ProcessFlyingPhysics(int32 nMode, float fTimeStep, float a4, float a5, float a6, float a7, float a8)
{
    uint8* self = reinterpret_cast<uint8*>(this);
    const float* pFly = *reinterpret_cast<const float* const*>(*reinterpret_cast<const uint8* const*>(self + 0xE18) + 0xFC);
    if (!pFly || fTimeStep <= 0.0f)
        return;

    auto dot = [](const CVector& a, const CVector& b) -> float
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    };
    auto cross = [](const CVector& a, const CVector& b) -> CVector
    {
        return {
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        };
    };
    auto clampVal = [](float v, float lo, float hi) -> float
    {
        return (v < lo) ? lo : ((v > hi) ? hi : v);
    };

    uint8* pCtrl = nullptr;
    if ((m_nFlags2 & 0x7C00u) == 0 && m_pDriver && m_pDriver->m_nPedUnk219 != 0)
        pCtrl = Core::Hook::CallMethod<uint8*>(0x9BE5D0, m_pDriver);

    CVector vel(0.0f, 0.0f, 0.0f);
    CVector wind(0.0f, 0.0f, 0.0f);
    if (m_nCreatedBy != 2)
    {
        Core::Hook::CallMethod<void>(0x8E03F0, reinterpret_cast<void*>(Core::Rebase(0x169F4D0)), &m_pMatrix->pos, &wind);
        vel = wind * (-pFly[14]);
    }

    float tmpVel[4] = {};
    const float* pMove = reinterpret_cast<float*(__thiscall*)(CVehicle*, float*)>(m_vftable[59])(this, tmpVel);
    vel.x += pMove[0];
    vel.y += pMove[1];
    vel.z += pMove[2];

    void* pPhInst = Core::Hook::CallMethod<void*>(0x9E86F0, this);
    float fMass = static_cast<float>(reinterpret_cast<double(__thiscall*)(void*)>((*reinterpret_cast<void***>(pPhInst))[9])(pPhInst));
    const float* pInertia = reinterpret_cast<const float*(__thiscall*)(void*)>((*reinterpret_cast<void***>(pPhInst))[11])(pPhInst);
    float ix = pInertia[0];
    float iy = pInertia[1];
    float iz = pInertia[2];

    if (nMode <= 0)
        return;

    const CVector& pMatRight = m_pMatrix->right;
    const CVector& pMatFwd = m_pMatrix->up;
    const CVector& pMatUp = m_pMatrix->at;
    const float sentinel = Core::StaticRef<float>(0xD82330);
    const float inputScale = Core::StaticRef<float>(0xD503CC);
    const float g = Core::StaticRef<float>(0xD8232C);

    if (nMode == 5)
    {
        if (a5 == sentinel)
            a5 = pCtrl ? static_cast<float>(Core::Hook::Call<int32>(0x834730, Core::Hook::CallMethod<void*>(0x839330, pCtrl))) * inputScale : 0.0f;

        if (a6 == sentinel)
            a6 = pCtrl ? static_cast<float>(-Core::Hook::CallMethod<int32>(0x839320, pCtrl)) * inputScale : 0.0f;

        if (a4 == sentinel)
        {
            a4 = 0.0f;
            if (pCtrl)
            {
                uint8 rA = static_cast<uint8>(pCtrl[0x29AC] ^ pCtrl[0x29AE]);
                uint8 rB = static_cast<uint8>(pCtrl[0x299C] ^ pCtrl[0x299E]);
                if (rA > 0x7Fu && rB <= 0x7Fu)
                    a4 = 1.0f;
                if (rB > 0x7Fu && rA <= 0x7Fu)
                    a4 = -1.0f;
                int32 v = Core::Hook::Call<int32>(0x834730, Core::Hook::CallMethod<void*>(0x839360, pCtrl));
                int32 av = (v < 0) ? -v : v;
                if (av > 1)
                    a4 = static_cast<float>(v) * inputScale;
            }
        }

        if (a7 == sentinel)
            a7 = pCtrl ? static_cast<float>(Core::Hook::Call<double>(0x8348B0, pCtrl + 0x2918) - Core::Hook::Call<double>(0x8348B0, pCtrl + 0x2928)) : 0.0f;

        float vUp = dot(pMatUp, vel);
        if (vUp < 0.0f)
            vUp *= 2.0f;

        float thr = a7;
        if (thr > 1.0f)
            thr = (thr - 1.0f) * pFly[0] + 1.0f;
        if (thr > 0.0f)
        {
            float ex = static_cast<float>(GetExcessSpeedOrInvalid(m_pMatrix->pos.z, 5));
            if (ex > 0.0f)
                thr = (thr / (ex + 10.0f)) * 10.0f;
        }

        float fLift = thr - pFly[1] * vUp;
        CVector liftForce = pMatUp * (fLift * g * a8 * fMass);
        AddVehUnk10A0(&liftForce.x);

        if (a5 < 0.0f || (dot(pMatFwd, vel) > 0.0f && Core::StaticRef<int32>(0x10F8044) >= 2 &&
                          (m_nModelIndex == Core::StaticRef<int32>(0x1230364) || m_nModelIndex == Core::StaticRef<int32>(0x1230AD8))))
        {
            float d = dot(pMatRight, vel);
            float px = vel.x - d * pMatRight.x;
            float py = vel.y - d * pMatRight.y;
            float h = std::sqrtf(px * px + py * py);
            if (h > 10.0f)
            {
                float s = clampVal(h - Core::StaticRef<float>(0xEBE088), 0.0f, 1.0f);
                float k = -(pFly[2] * s * a5) * g * fLift * a8 * fMass;
                CVector dragVec(pMatFwd.x * k, pMatFwd.y * k, 0.0f);
                AddVehUnk10A0(&dragVec.x);
            }
        }

        if (pMatUp.z > 0.0f)
        {
            CVector windUp = wind * pFly[14] + CVector(0.0f, 0.0f, 1.0f);
            Core::Hook::CallMethod<void>(0x42F5D0, &windUp);
            float t1 = -clampVal(dot(pMatRight, windUp), -pFly[10], pFly[10]) * pFly[11] * iy * a8;
            CVector v1 = pMatUp * t1;
            AddCrossVehUnk10B0(&v1.x, &pMatRight.x);

            float t2 = -clampVal(dot(pMatFwd, windUp), -pFly[10], pFly[10]) * pFly[11] * ix * a8;
            CVector v2 = pMatUp * t2;
            AddCrossVehUnk10B0(&v2.x, &pMatFwd.x);
        }
        else
        {
            float s1 = (pMatRight.z >= 0.0f) ? -pFly[10] : pFly[10];
            CVector v1 = pMatUp * (pFly[11] * s1 * iy * a8);
            AddCrossVehUnk10B0(&v1.x, &pMatRight.x);

            float s2 = (pMatFwd.z >= 0.0f) ? -pFly[10] : pFly[10];
            CVector v2 = pMatUp * (pFly[11] * s2 * ix * a8);
            AddCrossVehUnk10B0(&v2.x, &pMatFwd.x);
        }

        CVector cycPitch = pMatUp * (a5 * pFly[8] * ix * a8);
        AddCrossVehUnk10B0(&cycPitch.x, &pMatFwd.x);

        CVector cycRoll = pMatUp * (a6 * pFly[6] * iy * a8);
        AddCrossVehUnk10B0(&cycRoll.x, &pMatRight.x);

        CVector bone = Core::StaticRef<CVector>(0x18A8C60);
        CVehicleModelInfo* mi = static_cast<CVehicleModelInfo*>(CModelInfo::ms_modelInfoPtrs[m_nModelIndex]);
        if (*reinterpret_cast<const int32*>(reinterpret_cast<const uint8*>(mi->m_pVehicleStruct) + 0x16C) > -1 &&
            Core::Hook::CallMethod<void*>(0x447A20, this))
        {
            float bonePos[4] = {};
            GetBoneWorldPos(91, bonePos);
            bone = { bonePos[0], bonePos[1], bonePos[2] };
        }
        else
        {
            const float* pMin = reinterpret_cast<const float*(__thiscall*)(CVehicle*)>(m_vftable[24])(this);
            bone.z = pMin[2];
        }

        Core::Hook::CallMethod<void>(0x516C60, m_pMatrix, &bone);
        float tmpAng[4] = {};
        const float* pAngVel = Core::Hook::CallMethod<const float*>(0xA478E0, this, tmpAng);
        CVector angVel(pAngVel[0], pAngVel[1], pAngVel[2]);
        CVector tailVel = vel + cross(angVel, bone);
        float tailFwd = dot(pMatFwd, tailVel);
        float tailSpeedSq = tailVel.MagnitudeSquared();

        float slip = clampVal(-static_cast<float>(std::atan2(dot(pMatRight, tailVel), tailFwd)),
                              Core::StaticRef<float>(0xD82328),
                              Core::StaticRef<float>(0xD81F44));
        CVector sideForce = pMatRight * (clampVal(pFly[5] * slip * tailSpeedSq, -100.0f, 100.0f) * fMass);
        AddVehUnk10A0(&sideForce.x);

        CVector negFwd = pMatFwd * -1.0f;
        CVector yawForce = pMatRight * ((pFly[4] * slip * tailSpeedSq + pFly[3] * a4 * a8) * iz);
        AddCrossVehUnk10B0(&yawForce.x, &negFwd.x);

        float ang2 = clampVal(-static_cast<float>(std::atan2(dot(pMatUp, tailVel), tailFwd)),
                              Core::StaticRef<float>(0xD82328),
                              Core::StaticRef<float>(0xD81F44));
        CVector pitchForce = pMatUp * (pFly[9] * ang2 * tailSpeedSq * iy);
        AddCrossVehUnk10B0(&pitchForce.x, &negFwd.x);
        return;
    }

    if (nMode >= 1 && nMode <= 3)
    {
        if (a4 == sentinel)
            a4 = pCtrl ? static_cast<float>(Core::Hook::CallMethod<int32>(0x839320, pCtrl)) * inputScale : 0.0f;

        if (a5 == sentinel)
        {
            if (pCtrl)
            {
                a5 = static_cast<float>(Core::Hook::Call<int32>(0x834730, Core::Hook::CallMethod<void*>(0x839330, pCtrl))) * inputScale;
                int32 vRaw = Core::Hook::Call<int32>(0x834730, Core::Hook::CallMethod<void*>(0x8393A0, pCtrl));
                int32 vAbs = (vRaw < 0) ? -vRaw : vRaw;
                if (static_cast<float>(vAbs) > 1.0f)
                    a5 = static_cast<float>(-vRaw) * inputScale;
            }
            else
            {
                a5 = 0.0f;
            }
        }

        if (a7 == sentinel)
            a7 = pCtrl ? static_cast<float>(Core::Hook::Call<double>(0x8348B0, pCtrl + 0x2918) - Core::Hook::Call<double>(0x8348B0, pCtrl + 0x2928)) : 0.0f;

        float ang = static_cast<float>(std::atan2(static_cast<double>(a5), static_cast<double>(a4)));
        float scale = 1.0f;
        if (ang > -0.78539819f && ang <= 0.78539819f)
        {
            scale = 1.0f / static_cast<float>(std::cos(static_cast<double>(ang)));
        }
        else if (ang > 0.78539819f && ang <= 2.3561945f)
        {
            ang -= 1.5707964f;
            scale = 1.0f / static_cast<float>(std::cos(static_cast<double>(ang)));
        }
        else if (ang > 2.3561945f)
        {
            ang -= 3.1415927f;
            scale = 1.0f / static_cast<float>(std::cos(static_cast<double>(ang)));
        }
        else if (ang <= -2.3561945f)
        {
            ang += 3.1415927f;
            scale = 1.0f / static_cast<float>(std::cos(static_cast<double>(ang)));
        }
        else if (ang > -2.3561945f && ang < -0.78539819f)
        {
            ang += 1.5707964f;
            scale = 1.0f / static_cast<float>(std::cos(static_cast<double>(ang)));
        }

        float v210 = scale * a4;
        float v211 = -(scale * a5);

        float fwdSpeed = dot(pMatFwd, vel);
        float fwdSpeedSq = fwdSpeed * fwdSpeed;
        float fThrust = 0.0f;
        if (!reinterpret_cast<bool(__thiscall*)(CVehicle*)>(m_vftable[89])(this) && a7 <= 0.0f)
        {
            float tmpV[4] = {};
            if (a7 >= 0.0f || Core::Hook::Call<float>(0x458850, &m_pMatrix->up, reinterpret_cast<float*(__thiscall*)(CVehicle*, float*)>(m_vftable[59])(this, tmpV)) >= Core::StaticRef<float>(0xD529C4))
                fThrust = 0.0f;
            else
                fThrust = ((a7 - fwdSpeed * Core::StaticRef<float>(0xD82324)) < 0.0f ? (a7 - fwdSpeed * Core::StaticRef<float>(0xD82324)) : 0.0f) * pFly[0];
        }
        else
        {
            fThrust = (a7 + 1.0f) * pFly[0] * 0.5f;
            if (fwdSpeed > 0.0f && pFly[1] < 1.0f)
            {
                float k = (pFly[1] < 0.0f) ? 3.0f : 0.65f;
                float diff = (pFly[1] < 0.0f) ? (pFly[1] + fwdSpeed) : (fwdSpeed - pFly[1]);
                float factor = 1.0f - diff * diff * k;
                if (factor < 0.0f)
                    factor = 0.0f;
                fThrust *= factor;
            }
        }

        if (nMode == 2)
            fThrust *= 0.3f;
        else if (nMode == 3)
            fThrust *= 0.1f;

        CVector thrustVec = pMatFwd * (g * fThrust * fMass);
        AddVehUnk10A0(&thrustVec.x);

        float sDot = -pMatRight.y * vel.y - pMatRight.z * vel.z - pMatRight.x * vel.x;
        CVector sideDrag = pMatRight * (pFly[5] * std::fabsf(sDot) * sDot * fMass * g);
        AddVehUnk10A0(&sideDrag.x);

        const float* pMin = reinterpret_cast<const float*(__thiscall*)(CVehicle*)>(m_vftable[24])(this);
        CVector v188 = pMatFwd * pMin[1];

        float tmp140[4] = {};
        const float* v140 = Core::Hook::CallMethod<const float*>(0xA47FB0, this, tmp140, &v188, 0, 0);
        float yDot = -v140[1] * pMatRight.y - v140[2] * pMatRight.z - v140[0] * pMatRight.x;
        CVector yawVec = pMatRight * ((pFly[4] * std::fabsf(yDot) * yDot + pFly[3] * fwdSpeed * v210) * iz * g);
        AddCrossVehUnk10B0(&yawVec.x, &v188.x);

        float rollInput = (a6 == sentinel) ? v210 : a6;
        float rVal = (pFly[6] * iy * fwdSpeed) * rollInput * g;
        CVector rollVec = pMatRight * rVal;
        AddCrossVehUnk10B0(&rollVec.x, &pMatUp.x);

        CVector crossVec(-pMatFwd.y, pMatFwd.x, 0.0f);
        float sign = (pMatRight.z >= 0.0f) ? 1.0f : -1.0f;
        float v161 = (1.0f - dot(pMatRight, crossVec)) * sign;
        float v163 = (1.0f - std::fabsf(pMatFwd.z)) * v161 * pFly[7] * iy * Core::StaticRef<float>(0xEBE4DC);
        CVector levelVec = pMatRight * v163;
        AddCrossVehUnk10B0(&levelVec.x, &pMatUp.x);

        float tmp165[4] = {};
        const float* v165 = Core::Hook::CallMethod<const float*>(0xA47FB0, this, tmp165, &v188, 0, 0);
        float pDot = -pMatUp.y * v165[1] - pMatUp.z * v165[2] - pMatUp.x * v165[0];
        CVector pitchVec = pMatUp * ((pFly[9] * std::fabsf(pDot) * pDot + pFly[8] * fwdSpeed * v211) * ix * g);
        AddCrossVehUnk10B0(&pitchVec.x, &v188.x);

        float lenVel = std::sqrtf(vel.MagnitudeSquared());
        float denom = (lenVel > 0.01f) ? lenVel : 0.01f;
        float diveAng = -static_cast<float>(std::asin(static_cast<double>(clampVal(dot(pMatUp, vel) / denom, -1.0f, 1.0f))));
        if (GetVehicleType() == 5 && diveAng > Core::StaticRef<float>(0xD81F40))
            *reinterpret_cast<int32*>(self + 0x1F18) += Core::Hook::Call<int32>(0x448330);

        float liftK = pFly[10];
        if (GetVehicleType() == 5 && *reinterpret_cast<const float*>(self + 0x1F44) < 1.0f)
            liftK *= pFly[13];

        float fLiftForce = (pFly[11] * diveAng + liftK) * fwdSpeedSq * fMass * g;
        if (fLiftForce > fMass * g)
        {
            float ex = static_cast<float>(GetExcessSpeedOrInvalid(m_pMatrix->pos.z, nMode));
            if (ex > 0.0f)
            {
                float k = 1.0f - ex * 0.02f;
                if (k < 0.0f)
                    k = 0.0f;
                fLiftForce = k * fMass * g;
            }
        }
        CVector liftVec = pMatUp * fLiftForce;
        AddVehUnk10A0(&liftVec.x);
    }
}

void* __thiscall CVehicle::FindOrAllocateLink(int32 val, bool bCreate)
{
    for (uint32 i = 0; i < m_nVehUnkFF8; ++i)
    {
        if (*reinterpret_cast<const int32*>(m_vehSubObjFFC + i * 0x14 + 0xC) == val)
            return m_vehSubObjFFC + i * 0x14;
    }
    return bCreate ? AllocateLink(val) : nullptr;
}

void __thiscall CVehicle::ProcessLinks()
{
    for (uint32 i = 0; i < m_nVehUnkFF8; ++i)
        reinterpret_cast<CVehicleLink*>(m_vehSubObjFFC + i * 0x14)->Process();
}

void* __thiscall CVehicle::GetLinkForSeat(int32 seatSlot, int32 seatIdx)
{
    void* pEntry = GetModelSeatEntry(seatSlot);
    int32 linkVal = Core::Hook::CallMethod<int32>(0xBE6200, pEntry, this, seatIdx);
    return FindOrAllocateLink(linkVal, false);
}

void* __thiscall CVehicle::GetLinkForSeatSlot(int32 seatSlot)
{
    void* pEntry = GetModelSeatEntry(seatSlot);
    int32 linkVal = Core::Hook::CallMethod<int32>(0xBE6220, pEntry, this);
    const uint8* mi = reinterpret_cast<const uint8*>(CModelInfo::ms_modelInfoPtrs[m_nModelIndex]);
    if (*reinterpret_cast<const int32*>(mi + 0xC4) == 0x57)
        linkVal = 5;

    int32 vehType = GetVehicleType();
    if (vehType == 1)
    {
        if (linkVal == 7)
            linkVal = 5;
        else if (linkVal == 8)
            linkVal = 6;
    }
    if (vehType == 3)
    {
        if (linkVal == 5 || linkVal == 7)
            linkVal = 5;
        else if (linkVal == 6 || linkVal == 8)
            linkVal = 6;
    }

    return FindOrAllocateLink(linkVal, false);
}

void __thiscall CVehicle::GetUnkSeatOffset(float* pOutVec4, int32 doorOrSeat, int32 a4)
{
    int32 idx;
    switch (doorOrSeat)
    {
    case 5:  idx = 0; break;
    case 6:  idx = 1; break;
    case 7:  idx = 2; break;
    default: idx = 3; break;
    }

    CVehicleModelInfo* mi = static_cast<CVehicleModelInfo*>(CModelInfo::ms_modelInfoPtrs[m_nModelIndex]);
    int32 handlingId = *reinterpret_cast<const int32*>(reinterpret_cast<const uint8*>(mi) + 0xC4);
    if (handlingId == 0x54 && ((mi->m_nVehicleFlags >> 5) & 1u) == 0)
    {
        if (idx == 1)
            idx = 0;
        else if (idx == 3)
            idx = 2;
    }

    float tmp[4];
    Core::Hook::Call<float*>(0xCBBFD0, tmp, handlingId, idx, this, a4);
    pOutVec4[0] = -tmp[0];
    pOutVec4[1] = -tmp[1];
    pOutVec4[2] = -tmp[2];
    pOutVec4[3] = tmp[3];
}

bool __thiscall CVehicle::SaveStateSnapshot()
{
    int32 size = sizeof(CVehicleStateSnapshot);
    CVehicleStateSnapshot snap;
    snap.Save(this);
    Core::Hook::Call<void>(0x82DEA0, &size, 4);
    Core::Hook::Call<void>(0x82DEA0, &snap, sizeof(CVehicleStateSnapshot));
    return true;
}

bool __thiscall CVehicle::LoadStateSnapshot()
{
    int32 size = 0;
    CVehicleStateSnapshot snap;
    Core::Hook::Call<void>(0x82DEB0, &size, 4);
    Core::Hook::Call<void>(0x82DEB0, &snap, sizeof(CVehicleStateSnapshot));
    snap.Restore(this);
    return true;
}

bool __thiscall CVehicle::IsBoneNonZero(int32 boneSlot)
{
    CVehicleModelInfo* mi = static_cast<CVehicleModelInfo*>(CModelInfo::ms_modelInfoPtrs[m_nModelIndex]);
    int32 boneId = static_cast<int32>(mi->m_pVehicleStruct->m_nBones[boneSlot]);
    if (!m_pFragInstE14 || boneId == -1)
        return false;

    void* pMatSrc = Core::Hook::CallMethod<void*>(0x447A60, this, boneId);
    float mat[16];
    Core::Hook::CallMethod<void>(0x441ED0, mat, pMatSrc);
    return (mat[12] * mat[12] + mat[13] * mat[13] + mat[14] * mat[14]) > Core::StaticRef<float>(0xDECD60);
}

bool __thiscall CVehicle::ResetBoneChildIfNonZero(int32 boneSlot)
{
    bool ok = IsBoneNonZero(boneSlot);
    if (ok)
        ResetBoneChild(boneSlot);
    return ok;
}

void* __cdecl CVehicle::InitVehiclePool()
{
    void* pPool = Core::Hook::Call<void*>(0x4011D0, 0x1C);
    if (pPool)
    {
        pPool = Core::Hook::CallMethod<void*>(0xC72F10, pPool, 140, "Vehicles", 0x20D0);
        Core::StaticRef<void*>(0x1619240) = pPool;
    }
    else
    {
        Core::StaticRef<void*>(0x1619240) = nullptr;
    }
    return pPool;
}

void __thiscall CVehicle::ApplyHiddenBoneMask()
{
    int32 t = GetVehicleType();
    if (t != 0 && t != 2 && t != 1 && t != 4)
        return;

    int32 startSlot = (t == 4) ? 0x5F : 0x59;
    void* pFrag = m_pFragInstE14 ? m_pFragInstE14 : m_pPhysicsInst;
    if (pFrag)
    {
        uint8* v5 = Core::Hook::CallMethod<uint8*>(0x51DDD0, pFrag);
        CVehicleModelInfo* mi = static_cast<CVehicleModelInfo*>(CModelInfo::ms_modelInfoPtrs[m_nModelIndex]);
        int32 count = static_cast<int32>(v5[0x1F3]);
        void** arr = *reinterpret_cast<void***>(v5 + 0xD4);
        for (int32 i = 0; i < count; ++i)
        {
            for (int32 slot = startSlot; slot <= 0x62; ++slot)
            {
                int32 target = static_cast<int32>(mi->m_pVehicleStruct->m_nBones[slot]);
                if (target != -1 && (m_nVehBoneMaskE20 & (1u << (static_cast<uint8>(slot) - 88))) != 0)
                {
                    if (target == *reinterpret_cast<const int16*>(static_cast<const uint8*>(arr[i]) + 0xE))
                        Core::Hook::CallMethod<void>(0x518180, pFrag, i);
                }
            }
        }
    }

    if (GetVehicleType() == 0 && !IsConvertibleRoofClosedOrValid())
    {
        ResetBoneChildIfNonZero(0x1E);
        ResetBoneChildIfNonZero(0x1F);
    }
}

void __thiscall CVehicle::UpdateSubObj80Position()
{
    uint32 state = (m_nFlags2 >> 6) & 0xFu;
    if (state == 4 || state == 3 || state == 6 || state == 2)
    {
        void* pSub80 = reinterpret_cast<uint8*>(this) + 0x80;
        const float* pPos = m_pMatrix ? &m_pMatrix->pos.x : &m_vecPos.x;
        reinterpret_cast<void(__thiscall*)(void*, const float*, int32)>((*reinterpret_cast<void***>(pSub80))[1])(pSub80, pPos, 0);
    }
}

void __thiscall CVehicle::ProcessDriverDropWeapon()
{
    CPed* pDriver = m_pDriver;
    if (!pDriver || Core::StaticRef<uint8>(0x1724286) != 0 || pDriver->m_nPedUnk218 != 0 || pDriver->m_nPedUnk219 == 0)
        return;

    uint8* pCtrl = Core::Hook::CallMethod<uint8*>(0x9BE5D0, pDriver);
    if ((pCtrl[0x328C] && Core::Hook::CallMethod<bool>(0x79A960, pCtrl + 0x2938, 0x190, 0x80, 0xFF)) ||
        (pCtrl[0x328D] && Core::Hook::CallMethod<bool>(0x426780, pCtrl + 0x2F18)))
    {
        m_nVehFlagsF71 |= 1u;
    }
    else
    {
        m_nVehFlagsF71 = (m_nVehFlagsF71 & 0xFEu) | 2u;
    }

    if ((m_nVehFlagsF71 & 1u) != 0 && (m_nVehFlagsF71 & 2u) != 0)
    {
        if (Core::StaticRef<uint8>(0x1549118) != 0)
        {
            uint8* pDriverBytes = reinterpret_cast<uint8*>(pDriver);
            uint8* pWeaponComp = pDriverBytes + 0x2B0;
            uint32* pSlot = Core::Hook::Call<uint32*>(0x924E80, *reinterpret_cast<uint32*>(pWeaponComp + 12 * (*reinterpret_cast<const int32*>(pDriverBytes + 0x2C8) + 5)));
            if (Core::Hook::CallMethod<bool>(0x9252D0, pSlot, pDriver))
            {
                Core::Hook::CallMethod<void>(0x9ABFD0, pWeaponComp, pDriver, *pSlot, 1);
                Core::Hook::CallMethod<void>(0x89E3E0, pDriver);
                if (!Core::Hook::Call<bool>(0x405180))
                {
                    uint8 buf[72];
                    Core::Hook::CallMethod<void>(0x797520, buf);
                    int32 id = Core::Hook::Call<int32>(0x799A90);
                    int32 sub = Core::Hook::Call<int32>(0x799CC0, id);
                    if (Core::Hook::CallMethod<bool>(0xCE8E70, reinterpret_cast<void*>(Core::Rebase(0x10F85B0)), "FRONTEND_GAME_DROP_WEAPON", buf, id, sub, 0))
                    {
                        int32 arr[3] = { 0, -1, 0x84 };
                        uint32 h = Core::Hook::Call<uint32>(0x5B1C30, "FRONTEND_GAME_DROP_WEAPON", 0);
                        Core::Hook::Call<void>(0x7A9810, h, 0, 0, 1, buf, arr, 0, id);
                        m_nVehFlagsF71 &= ~2u;
                        return;
                    }
                    Core::Hook::Call<void>(0x799B20, id);
                }
            }
        }
        m_nVehFlagsF71 &= ~2u;
    }
}

void __thiscall CVehicle::ProcessCollisions(void** pContacts, void* pEntity, int32, int32 count)
{
    int32 wheelCount = static_cast<int32>(m_nVehUnkFD4);
    for (int32 i = 0; i < count; ++i)
    {
        uint8* colBytes = static_cast<uint8*>(pContacts[i]);
        uint32* colInfo = *reinterpret_cast<uint32**>(colBytes + 0xA0);

        bool handledByWheel = false;
        int32 targetBone = (pEntity == reinterpret_cast<void*>(colInfo[13])) ? static_cast<int32>(colInfo[26]) : static_cast<int32>(colInfo[28]);
        for (int32 j = 0; j < wheelCount; ++j)
        {
            uint8* wheel = reinterpret_cast<uint8*>(m_nVehUnkFD0) + j * 0x170;
            if (*reinterpret_cast<const int16*>(wheel + 4) == targetBone &&
                Core::Hook::CallMethod<bool>(0xB44260, wheel, this, pContacts, pEntity, i, count))
            {
                handledByWheel = true;
                break;
            }
        }
        if (handledByWheel)
            continue;

        CVector normal = *reinterpret_cast<const CVector*>(colBytes + 0x50);
        if (pEntity != reinterpret_cast<void*>(colInfo[13]))
        {
            normal.x = -normal.x;
            normal.y = -normal.y;
            normal.z = -normal.z;
        }

        uint8* other = (pEntity == reinterpret_cast<void*>(colInfo[13]))
            ? reinterpret_cast<uint8*>(colInfo[14])
            : reinterpret_cast<uint8*>(colInfo[13]);
        uint8* v19 = other ? *reinterpret_cast<uint8**>(other + 0xC) : nullptr;
        if (v19 && (*reinterpret_cast<const uint32*>(v19 + 0x28) & 0x3C0u) == 0x80u)
        {
            float fwdDot = m_pMatrix->up.x * normal.x + m_pMatrix->up.y * normal.y + m_pMatrix->up.z * normal.z;
            if (std::fabsf(fwdDot) > Core::StaticRef<float>(0xDCE03C))
            {
                float upDot = m_pMatrix->at.x * normal.x + m_pMatrix->at.y * normal.y + m_pMatrix->at.z * normal.z;
                normal.x -= m_pMatrix->at.x * upDot;
                normal.y -= m_pMatrix->at.y * upDot;
                normal.z -= m_pMatrix->at.z * upDot;
                float lenSq = normal.MagnitudeSquared();
                if (lenSq < Core::StaticRef<float>(0xEBE568) || lenSq > Core::StaticRef<float>(0xEBE710))
                {
                    float invLen = (lenSq == 0.0f) ? 0.0f : (1.0f / std::sqrtf(lenSq));
                    normal.x *= invLen;
                    normal.y *= invLen;
                    normal.z *= invLen;
                }
                CVector* pNormA = reinterpret_cast<CVector*>(colBytes + 0x50);
                CVector* pNormB = reinterpret_cast<CVector*>(colBytes + 0x60);
                if (pEntity == reinterpret_cast<void*>(colInfo[13]))
                {
                    *pNormA = normal;
                    *pNormB = normal * -1.0f;
                }
                else
                {
                    *pNormA = normal * -1.0f;
                    *pNormB = normal;
                }
            }

            if (std::fabsf(m_pMatrix->up.x * normal.x + m_pMatrix->up.y * normal.y + m_pMatrix->up.z * normal.z) > Core::StaticRef<float>(0xD503BC))
            {
                float v37[3];
                Core::Hook::CallMethod<void>(0x6952C0, pContacts[i], v37);
                if (m_pMatrix->up.x * v37[0] + m_pMatrix->up.y * v37[1] + m_pMatrix->up.z * v37[2] < Core::StaticRef<float>(0xEBE0FC))
                    *reinterpret_cast<float*>(colBytes + 0x1C) *= Core::StaticRef<float>(0xDCD0EC);
            }
        }
        else
        {
            float upDot = m_pMatrix->at.x * normal.x + m_pMatrix->at.y * normal.y + m_pMatrix->at.z * normal.z;
            if (normal.z > Core::StaticRef<float>(0xD503BC) && upDot > Core::StaticRef<float>(0xD503BC))
                *reinterpret_cast<float*>(colBytes + 0x1C) *= Core::StaticRef<float>(0xD50964);
            else if (m_pMatrix->at.z < 0.0f)
                *reinterpret_cast<float*>(colBytes + 0x1C) *= Core::StaticRef<float>(0xD8200C);
        }
    }
}

void __thiscall CVehicle::InitBrokenBoneMask(int16 flags)
{
    m_nVehBoneMaskE20 = 0;
    int32 t = GetVehicleType();
    if (t != 0 && t != 2 && t != 1)
        return;

    CVehicleModelInfo* mi = static_cast<CVehicleModelInfo*>(CModelInfo::ms_modelInfoPtrs[m_nModelIndex]);
    uint32 m6C = static_cast<uint32>(mi->m_nVehicleType);
    uint32 m94 = mi->m_nVehicleFlags;

    uint32 v48 = 0, v38 = 0, v43 = 0, v45 = 0, v37 = 0xFFFFFFFFu;
    if (m6C == 1)
    {
        v38 = Core::StaticRef<uint32>(0xF33A88);
        v43 = Core::StaticRef<uint32>(0xF33A8C);
        v45 = Core::StaticRef<uint32>(0xF33A90);
        v37 = Core::StaticRef<uint32>(0xF33A94);
    }
    else if (m94 & 0x10000u)
    {
        if (flags & 0x200)
        {
            v48 = Core::StaticRef<uint32>(0xF33A70);
            v38 = 0;
            v37 = 0;
        }
        else if (m94 & 0x800000u)
        {
            v48 = ~Core::StaticRef<uint32>(0xF33A70);
            v37 = 0;
        }
        else if (m94 & 0x100000u)
        {
            v38 = Core::StaticRef<uint32>(0xF33A6C);
            v37 = 0;
        }
        else
        {
            v37 = Core::StaticRef<uint32>(0xF33A6C);
        }
    }
    else if ((m94 & 0x20000u) || (m94 & 0x1000000u))
    {
        uint32 base = Core::StaticRef<uint32>(0xF33A74);
        v38 = (flags & 0x100) ? Core::StaticRef<uint32>(0xF33A78) : ((flags & 0x400) ? Core::StaticRef<uint32>(0xF33A7C) : base);
        if (m94 & 0x800000u)
        {
            v48 = ~v38;
            v37 = 0;
        }
        else
        {
            v37 = ~base;
        }
    }
    else if (m94 & 0x40000u)
    {
        v38 = Core::StaticRef<uint32>(0xF33A80);
        if (m94 & 0x800000u)
        {
            v48 = v38;
            v37 = 0;
        }
        else
        {
            v37 = ~v38;
        }
    }
    else if (m94 & 0x100000u)
    {
        v38 = 0xFFFFFFFFu;
        v37 = 0;
    }
    else if (m94 & 0x800000u)
    {
        v48 = 0xFFFFFFFFu;
        v37 = 0;
    }

    if (m94 & 0x400000u)
    {
        uint32 m = ~Core::StaticRef<uint32>(0xF33A84);
        v38 &= m;
        v37 &= m;
    }
    if (m6C == 0)
    {
        v48 &= 0xFFFFFDFFu;
        v38 &= 0xFFFFFDFFu;
        v37 &= 0xFFFFFDFFu;
    }

    int32* pCC = reinterpret_cast<int32*>(mi->m_pVehicleStruct);
    int32 total = 0, c38 = 0, c43 = 0, c45 = 0, c37 = 0;
    for (int32 j = 0; j <= 9; ++j)
    {
        if (pCC[89 + j] != -1)
        {
            ++total;
            uint32 bit = 1u << (1 + j);
            if (bit & v38) ++c38;
            if (bit & v43) ++c43;
            if (bit & v45) ++c45;
            if (bit & v37) ++c37;
        }
    }
    if (total <= 0)
        return;

    const float randScale = Core::StaticRef<float>(0xEBE1D8);
    int32 r38 = (c38 > 0) ? static_cast<int32>(static_cast<float>(static_cast<uint16>(Core::Hook::Call<int32>(0xD0D21A))) * randScale * static_cast<float>(c38)) + 1 : -1;
    int32 r43 = (c43 > 0) ? static_cast<int32>(static_cast<float>(static_cast<uint16>(Core::Hook::Call<int32>(0xD0D21A))) * randScale * static_cast<float>(c43)) + 1 : -1;
    int32 r45 = (c45 > 0) ? static_cast<int32>(static_cast<float>(static_cast<uint16>(Core::Hook::Call<int32>(0xD0D21A))) * randScale * static_cast<float>(c45)) + 1 : -1;
    int32 r37 = -1;
    if (c37 > 0)
    {
        float mult = (m94 & 0x80000u) ? static_cast<float>(4 * c37 + 1) : static_cast<float>(c37 + 1);
        r37 = static_cast<int32>(static_cast<float>(static_cast<uint16>(Core::Hook::Call<int32>(0xD0D21A))) * randScale * mult);
        if (r37 > c37)
            r37 = 0;
    }

    for (int32 j = 0; j <= 9; ++j)
    {
        if (pCC[89 + j] != -1)
        {
            uint32 bit = 1u << (1 + j);
            if (!(bit & v48))
            {
                if (bit & v38) { if (r38-- != 1) m_nVehBoneMaskE20 |= bit; }
                else if (bit & v43) { if (r43-- != 1) m_nVehBoneMaskE20 |= bit; }
                else if (bit & v45) { if (r45-- != 1) m_nVehBoneMaskE20 |= bit; }
                else if (bit & v37) { if (r37-- != 1) m_nVehBoneMaskE20 |= bit; }
                else { m_nVehBoneMaskE20 |= bit; }
            }
        }
    }

    void* pFrag = m_pFragInstE14 ? m_pFragInstE14 : m_pPhysicsInst;
    if (pFrag)
    {
        void* v29 = reinterpret_cast<void*(__thiscall*)(void*)>((*reinterpret_cast<void***>(pFrag))[56])(pFrag);
        if (v29)
        {
            uint8* v30 = *reinterpret_cast<uint8**>(static_cast<uint8*>(v29) + 4);
            for (int32 slot = 89; slot <= 98; ++slot)
            {
                int32 id = pCC[slot];
                if (id != -1 && (m_nVehBoneMaskE20 & (1u << (slot - 88))) == 0)
                {
                    uint16 childId = *reinterpret_cast<const uint16*>(*reinterpret_cast<const uint8* const*>(*reinterpret_cast<const uint8* const*>(v30) + 224 * id + 16) + 20);
                    for (int32 k = 89; k <= 98; ++k)
                    {
                        if (pCC[k] == static_cast<int32>(childId))
                        {
                            m_nVehBoneMaskE20 &= ~(1u << (k - 88));
                            break;
                        }
                    }
                }
            }
        }
    }
}

void __thiscall CVehicle::UpdateDoorOpenRatio(void*, int32 doorId, uint8* pBone, bool bFlag)
{
    if (GetVehicleType() == 3)
        return;

    void* pDoor = FindSubObjFD8(doorId);
    if (!pDoor)
        return;

    float cur = *reinterpret_cast<const float*>(pBone + 0x4C);
    void* ctx = (*reinterpret_cast<const uint16*>(pBone + 0x44) == 1) ? *reinterpret_cast<void* const*>(pBone + 0x40) : nullptr;
    if (!Core::Hook::CallMethod<bool>(0x58ABB0, ctx, 0x80, 0))
        return;

    float fMin = 0.0f, fMax = 1.0f;
    int32 miss = 0;
    if (!Core::Hook::CallMethod<bool>(0xA4C8F0, pBone, 0x10000u, &fMin, 0.0f, 1.0f))
    {
        fMin = 0.0f;
        ++miss;
    }
    if (!Core::Hook::CallMethod<bool>(0xA4C8F0, pBone, 0x20000u, &fMax, 0.0f, 1.0f))
    {
        fMax = 1.0f;
        ++miss;
    }
    if (miss == 2)
    {
        if (!bFlag)
            return;
        fMax = 0.0f;
    }

    if (cur >= fMax)
    {
        if (GetVehicleType() == 1)
            *(reinterpret_cast<uint8*>(this) + 0x1360) |= 8u;
        Core::Hook::CallMethod<void>(0xA5DCC0, pDoor, 1.0f, 0x200u);
    }
    else if (cur >= fMin)
    {
        if (GetVehicleType() == 1)
            *(reinterpret_cast<uint8*>(this) + 0x1360) |= 8u;
        float r = (cur - fMin) / (fMax - fMin);
        if (r > 1.0f) r = 1.0f;
        float existing = *reinterpret_cast<const float*>(static_cast<const uint8*>(pDoor) + 0x1C);
        if (existing > r) r = existing;
        Core::Hook::CallMethod<void>(0xA5DCC0, pDoor, r, 0x200u);
    }
}

void __thiscall CVehicle::UpdateDoorCloseRatio(void*, int32 doorId, uint8* pBone, bool bFlag)
{
    void* pDoor = FindSubObjFD8(doorId);
    if (!pDoor)
        return;

    float cur = *reinterpret_cast<const float*>(pBone + 0x4C);
    void* ctx = (*reinterpret_cast<const uint16*>(pBone + 0x44) == 1) ? *reinterpret_cast<void* const*>(pBone + 0x40) : nullptr;
    if (!Core::Hook::CallMethod<bool>(0x58ABB0, ctx, 0x80, 0))
        return;

    float fMin = 0.0f, fMax = 1.0f;
    int32 miss = 0;
    if (!Core::Hook::CallMethod<bool>(0xA4C8F0, pBone, 0x10000u, &fMin, 0.0f, 1.0f))
    {
        fMin = 0.0f;
        ++miss;
    }
    if (!Core::Hook::CallMethod<bool>(0xA4C8F0, pBone, 0x20000u, &fMax, 0.0f, 1.0f))
    {
        fMax = 1.0f;
        ++miss;
    }
    if (miss == 2)
    {
        if (!bFlag)
            return;
        fMin = fMax = 0.0f;
    }

    if (cur >= fMax)
    {
        Core::Hook::CallMethod<void>(0xA5DCC0, pDoor, 0.0f, 0x20200u);
    }
    else if (cur >= fMin)
    {
        float r = 1.0f - (cur - fMin) / (fMax - fMin);
        if (r > 1.0f) r = 1.0f;
        float existing = *reinterpret_cast<const float*>(static_cast<const uint8*>(pDoor) + 0x1C);
        if (existing < r) r = existing;
        Core::Hook::CallMethod<void>(0xA5DCC0, pDoor, r, 0x20200u);
    }
}

void __thiscall CVehicle::UpdateUnk10D8_10C8_10CC(float* pVal1, int32, float* pVal2, float* pVal3, bool bActive, bool bFlag)
{
    if (!bActive && !bFlag)
        return;

    float dt = CTimer::ms_fFrameTimeStep;
    float step1 = dt * (bActive ? 2.0f : 4.0f);
    if (std::fabsf(*pVal1 - m_fVehUnk10D8) >= step1)
        *pVal1 += (*pVal1 < m_fVehUnk10D8) ? step1 : -step1;
    else
        *pVal1 = m_fVehUnk10D8;

    if (bActive)
    {
        float diff2 = m_fVehUnk10C8 - *pVal2;
        float step2 = dt * (diff2 <= 0.0f ? 10.0f : 1.0f);
        if (std::fabsf(diff2) >= step2)
            *pVal2 += (diff2 > 0.0f) ? step2 : -step2;
        else
            *pVal2 = m_fVehUnk10C8;

        float cap = ((m_pMatrix->up.z * 4.0f > 0.0f) ? m_pMatrix->up.z * 4.0f : 0.0f) +
                    static_cast<float>(Core::Hook::Call<double>(0xBE6070, m_pDriver));
        if (*pVal2 > cap)
            *pVal2 = cap;

        float diff3 = m_fVehUnk10CC - *pVal3;
        float step3 = dt * (diff3 > 0.0f ? 10.0f : 1.0f);
        if (std::fabsf(diff3) >= step3)
            *pVal3 += (diff3 > 0.0f) ? step3 : -step3;
        else
            *pVal3 = m_fVehUnk10CC;
    }
}

void __thiscall CVehicle::SetEngineOrBurnoutState(bool bEnable)
{
    if (bEnable)
    {
        m_nVehFlagsF64 = (m_nVehFlagsF64 & 0xA7u) | 0x48u;
    }
    else if ((m_nVehFlagsF64 & 0x18u) == 0)
    {
        m_nVehFlagsF64 |= 0x10u;
        uint8* p6C = reinterpret_cast<uint8*>(m_nUnk6C);
        if (p6C && p6C[0xE] != 0)
        {
            m_nVehFlagsF64 |= 0x20u;
        }
        else
        {
            bool ok = Core::Hook::Call<bool>(0xCDBD10, static_cast<float>(GetBurnoutOrEngineHealthFactor()));
            if (ok)
                m_nVehFlagsF64 |= 0x20u;
            else
                m_nVehFlagsF64 &= ~0x20u;
        }
        Core::Hook::CallMethod<double>(0xAEA700, m_vehSubObj260, (m_nVehFlagsF64 & 0x20u) == 0);
        m_nVehUnkF88 = CTimer::ms_nTimeMs;
    }
}

void __thiscall CVehicle::ProcessEngineOrBurnout()
{
    uint8* self = reinterpret_cast<uint8*>(this);
    if ((m_nVehFlagsF64 & 0x10u) != 0 &&
        (!m_pDriver || Core::Hook::CallMethod<bool>(0x426910, static_cast<uint8*>(m_pDriver->m_pPedIntelligence) + 0x2E0, 0x2E2, 0)))
    {
        m_nVehFlagsF64 &= ~0x10u;
        m_nVehUnk1344 = 0;
    }

    if ((m_nVehFlagsF64 & 0x10u) != 0 &&
        Core::Hook::CallMethod<double>(0x8AE610, m_vehSubObj1120) > 0.0 &&
        (m_nFlags2 & 0x7C00u) == 0 &&
        m_pDriver)
    {
        if (Core::Hook::CallMethod<int32>(0x89DD90, m_pDriver, 5.0f) >= 1)
        {
            m_nVehFlagsF64 = (m_nVehFlagsF64 & 0xE7u) | 8u;
        }
        else
        {
            void* t = Core::Hook::CallMethod<void*>(0x426850, static_cast<uint8*>(m_pDriver->m_pPedIntelligence) + 0x44, 0x2C5);
            if (t && *reinterpret_cast<const int32*>(static_cast<const uint8*>(t) + 0x48) >= 9)
                m_nVehFlagsF64 = (m_nVehFlagsF64 & 0xE7u) | 8u;
        }
    }

    if ((m_nVehFlagsF64 & 0x10u) != 0)
    {
        int32 delay = static_cast<int32>(*reinterpret_cast<const float*>(self + 0xC84) * -1000.0f);
        if (CTimer::ms_nTimeMs > static_cast<uint32>(static_cast<int32>(m_nVehUnkF88) - delay))
        {
            if ((m_nVehFlagsF64 & 0x20u) != 0)
            {
                uint8 c = ++m_nVehUnk1344;
                ClearUnkFlagsF64_1344();
                if (Core::Hook::CallMethod<double>(0x8AE610, m_vehSubObj1120) > 0.0)
                {
                    Core::Hook::CallMethod<void>(0xAFBAF0, m_vehSubObj260);
                    m_nVehUnk1344 = c;
                    SetEngineOrBurnoutState(false);
                }
            }
            else
            {
                m_nVehFlagsF64 = (m_nVehFlagsF64 & 0xE7u) | 8u;
            }
        }
    }

    float mult = (m_nVehFlagsF6F & 1u) ? 0.4000000059604645f : 0.10000000149011612f;
    float env = *reinterpret_cast<const float*>(Core::Rebase(0x11F5D34) + Core::StaticRef<int32>(0x10F9AC0) * 0x210);
    m_fVehUnkF84 += (env - m_fVehUnkF84) * CTimer::ms_fFrameTimeStep * mult * 0.07000000029802322f;

    if ((m_nVehFlagsF64 & 8u) != 0)
    {
        float tmp[4];
        const float* spd = reinterpret_cast<float*(__thiscall*)(CVehicle*, float*)>(m_vftable[59])(this, tmp);
        const CVector& fwd = m_pMatrix->up;
        float maxSpd = *reinterpret_cast<const float*>(*reinterpret_cast<const uint8* const*>(self + 0xE18) + 0x4C);
        float r = std::fabsf(spd[0] * fwd.x + spd[1] * fwd.y + spd[2] * fwd.z) / maxSpd;
        r = (r < 0.0f) ? 0.0f : ((r > 1.0f) ? 1.0f : r);
        m_fVehUnkF84 += ((120.0f - r * 60.0f) - m_fVehUnkF84) * CTimer::ms_fFrameTimeStep * 0.07000000029802322f;
    }

    if (static_cast<int8>(m_nVehFlagsF6E) < 0 && !(m_nVehFlagsF6F & 1u) && m_fVehUnkF84 > 107.0f)
        m_nVehFlagsF6F |= 1u;
    else if ((m_nVehFlagsF6F & 1u) && m_fVehUnkF84 < 80.0999984741211f)
        m_nVehFlagsF6F &= ~1u;

    if ((m_pDriver && m_pDriver->m_nPedUnk219 != 0) || (m_pPassengers[0] && m_pPassengers[0]->m_nPedUnk219 != 0))
    {
        float v = m_fVehUnkFC4 + CTimer::ms_fFrameTimeStep;
        m_fVehUnkFC4 = (v > 1.0f) ? 1.0f : v;
    }
    else
    {
        float v = m_fVehUnkFC4 - CTimer::ms_fFrameTimeStep;
        m_fVehUnkFC4 = (v < 0.0f) ? 0.0f : v;
    }
}

void __thiscall CVehicle::ProcessInteriorLightOrUnk1114(float fTimeStep)
{
    uint8* self = reinterpret_cast<uint8*>(this);
    self[0xFE8] = 0xFF;
    uint16 timer = static_cast<uint16>(m_nVehUnk1114);
    if (timer == 0)
        return;

    Core::Hook::CallStdcall<void>(0xA48120, this, 40);
    uint16 dec = static_cast<uint16>(static_cast<int32>(fTimeStep * 1000.0f));
    timer = (timer <= dec) ? 0 : static_cast<uint16>(timer - dec);
    m_nVehUnk1114 = static_cast<int16>(timer);

    if (GetVehicleType() != 2)
    {
        float s = static_cast<float>(std::sin(static_cast<double>(timer & 0x1FFu) * 0.012271846644580364f));
        float v = ((s + 1.0f) * 0.5f) * 200.0f + 20.0f;
        if (v < 0.0f) v = 0.0f;
        if (v > 255.0f) v = 255.0f;
        uint8 b = (timer == 0) ? 0xFF : static_cast<uint8>(static_cast<int32>(v));
        self[0xFE8] = b;
        m_nUnk63 = static_cast<int8>(b);
    }

    if (GetVehicleType() != 1 && m_pDriver)
    {
        if (timer)
            m_pDriver->m_nFlags &= ~0x20u;
        else
            m_pDriver->m_nFlags |= 0x20u;
    }

    if (m_pDriver)
    {
        m_pDriver->m_nFlags |= 0x20u;
        m_nUnk63 = static_cast<int8>(self[0xFE8]);
    }
}

void __thiscall CVehicle::UpdateDirtLevel()
{
    float s1 = Core::StaticRef<float>(0xF33AE8);
    float scale = (s1 >= 1.0f) ? s1 : Core::StaticRef<float>(0xF33AE4);
    float vel[4];
    const float* pVel = reinterpret_cast<float*(__thiscall*)(CVehicle*, float*)>(m_vftable[59])(this, vel);
    float spd = std::sqrtf(pVel[0] * pVel[0] + pVel[1] * pVel[1]);
    float dirt = m_fVehUnk1118 + spd * scale * CTimer::ms_fFrameTimeStep;
    float maxDirt = Core::StaticRef<float>(0xD50960);
    m_fVehUnk1118 = (dirt > maxDirt) ? maxDirt : dirt;
}

void __thiscall CVehicle::Dtor()
{
    m_vftable = reinterpret_cast<void**>(Core::Rebase(0xD82164));

    for (int32 i = 0; i <= 3; ++i)
    {
        CPed* p = GetPassenger(i);
        if (!p)
            continue;
        void* p6C = reinterpret_cast<void*>(p->m_nUnk6C);
        if ((p6C && !reinterpret_cast<bool(__thiscall*)(void*)>((*reinterpret_cast<void***>(p6C))[24])(p6C)) ||
            reinterpret_cast<bool(__thiscall*)(CPed*)>(p->m_vftable[74])(p))
        {
            Core::Hook::CallMethod<void>(0x9C4DF0, p->m_pPedIntelligence, 1);
        }
        else
        {
            reinterpret_cast<void(__thiscall*)(CPed*)>(p->m_vftable[38])(p);
        }
    }

    Core::Hook::Call<void>(0x79F110, this);
    m_nVehUnkF98 = 0;
    ResetSubObjAndAudio();

    int32 id = Core::Hook::CallMethod<int32>(0x447230, Core::StaticRef<void*>(0x1619240), this);
    Core::Hook::Call<void>(0x80E6A0, id, 1);
    Core::Hook::Call<void>(0x83B3E0, this, 1);

    if (m_nVehFlagsF64 & 1u)
    {
        --Core::StaticRef<int32>(0x11DC81C);
        m_nVehFlagsF64 &= ~1u;
    }
    if (m_nVehFlagsF64 & 2u)
    {
        --Core::StaticRef<int32>(0x11DC820);
        m_nVehFlagsF64 &= ~2u;
    }

    if (m_nVehUnkF78 >= 0)
    {
        Core::Hook::Call<void>(0x80E620, m_nVehUnkF78, 1);
        m_nVehUnkF78 = -1;
    }

    m_fVehFadeScale111C = -1.0f;

    if (m_nVehFlagsF70 & 0x40u)
    {
        uint8* mi = reinterpret_cast<uint8*>(CModelInfo::ms_modelInfoPtrs[m_nModelIndex]);
        if (mi)
        {
            int32 altModel = *reinterpret_cast<const int32*>(mi + 0x34C);
            if (altModel > 0 && Core::Hook::Call<bool>(0x832DD0, altModel, CModelInfo::ms_nStreamingModuleId))
            {
                Core::Hook::CallMethod<void>(0x98E470, CModelInfo::ms_modelInfoPtrs[altModel]);
                void* pFEC = reinterpret_cast<void*>(m_nVehUnkFEC);
                if (pFEC)
                    reinterpret_cast<void(__thiscall*)(void*, int32)>((*reinterpret_cast<void***>(pFEC))[0])(pFEC, 1);
                m_nVehUnkFEC = 0;
            }
        }
    }

    m_nVehFlagsF71 |= 0x20u;

    Core::Hook::CallMethod<void>(0x8AE600, m_vehSubObj1120);
    Core::Hook::CallMethod<void>(0xC7E560, m_vehSubObj10E0);

    for (int32 j = 7; j >= 0; --j)
        reinterpret_cast<CVehicleLink*>(m_vehSubObjFFC + j * 0x14)->Dtor();

    Core::Hook::CallMethod<void>(0xA34400, m_vehSubObjE24);

    *reinterpret_cast<void**>(m_audPlaceableTracker) = reinterpret_cast<void*>(Core::Rebase(0xD69690));
    Core::Hook::CallMethod<void>(0xCF0E90, m_audPlaceableTracker);

    *reinterpret_cast<void**>(m_vehAudioEntity) = reinterpret_cast<void*>(Core::Rebase(0xD5295C));
    Core::Hook::CallMethod<void>(0xCE83F0, m_vehAudioEntity);

    Core::Hook::CallMethod<void>(0xAF0990, m_vehSubObj260);
    Core::Hook::CallMethod<void>(0xA47570, this);
}

void __thiscall CVehicle::SetModelIndex(int32 modelIndex)
{
    uint8* self = reinterpret_cast<uint8*>(this);
    *reinterpret_cast<uint32*>(self + 0x210) = 0x200u;

    uint8* mi = reinterpret_cast<uint8*>(CModelInfo::ms_modelInfoPtrs[modelIndex]);
    Core::Hook::CallMethod<void>(0x9E7C20, this, modelIndex);
    Core::StaticRef<uint32>(0x1724200) = 0;

    uint8* pH = Core::Hook::Call<uint8*>(0x9719C0, *reinterpret_cast<const int32*>(mi + 0x8C));
    *reinterpret_cast<uint8**>(self + 0xE18) = pH;
    *reinterpret_cast<uint32*>(self + 0xE1C) = *reinterpret_cast<const uint32*>(pH + 0xF0);
    *reinterpret_cast<float*>(self + 0x120) = *reinterpret_cast<const float*>(pH + 0x34);
    Core::StaticRef<uint32>(0x1724210) = 0x12u;

    uint32 hFlags = *reinterpret_cast<const uint32*>(pH + 0xEC);
    if (hFlags & 1u) m_nVehFlagsF65 |= 4u;
    if (hFlags & 8u) m_nVehFlagsF65 |= 0x10u;
    if (hFlags & 2u) m_nVehFlagsF65 |= 8u;
    if (hFlags & 4u) m_nVehFlagsF65 |= 0x20u;

    if (hFlags & 0x10u)
    {
        m_nVehUnkFE0 |= 0x10000u;
    }
    else if ((hFlags & 0x20u) && static_cast<float>(Core::Hook::Call<int32>(0xD0D21A)) * 3.0518509447574615e-05f > 0.5f)
    {
        m_nVehUnkFE0 |= 0x10000u;
    }
    else if ((hFlags & 0x40u) || ((hFlags & 0x80u) && static_cast<float>(Core::Hook::Call<int32>(0xD0D21A)) * 3.0518509447574615e-05f > 0.5f))
    {
        m_nVehUnkFE0 |= 0x20000u;
    }

    Core::StaticRef<void*>(0x1724218) = self + 0x210;
    int32 mA4 = *reinterpret_cast<const int32*>(mi + 0xA4);
    *reinterpret_cast<int32*>(self + 0xFC8) = mA4;
    m_nVehFlagsF6E ^= (m_nVehFlagsF6E ^ ((mA4 == 5) ? 2 : 0)) & 2u;

    Core::Hook::CallMethod<void>(0xBE6930, mi, self + 0xFE4, self + 0xFE5, self + 0xFE6, self + 0xFE7, 1);
    self[0xFE8] = 0xFF;

    uint32 mBC = *reinterpret_cast<const uint32*>(mi + 0xBC);
    uint32 r = static_cast<uint32>(Core::Hook::Call<int32>(0xD0D21A)) % (16u - mBC) + mBC;
    m_fVehUnk1118 = *reinterpret_cast<const uint32*>(mi + 0x84) ? static_cast<float>(r) : 0.0f;

    reinterpret_cast<void(__thiscall*)(CVehicle*)>(m_vftable[42])(this);
    reinterpret_cast<void(__thiscall*)(CVehicle*)>(m_vftable[16])(this);
    Core::Hook::CallMethod<void>(0x9E17B0, this);
    self[0xF4] |= 2u;

    m_nMaxPassengers = Core::Hook::Call<uint8>(0xBE6A60, modelIndex);

    int32 seatCount = *reinterpret_cast<const int32*>(mi + 0x3BC);
    for (int32 i = 0; i < seatCount; ++i)
    {
        void* entry = mi + 0x350 + i * 12;
        FindOrAllocateLink(Core::Hook::CallMethod<int32>(0xBE6220, entry, this), true);
        FindOrAllocateLink(Core::Hook::CallMethod<int32>(0xBE6200, entry, this, 0), true);
        if (Core::Hook::CallMethod<int32>(0x8EFD40, entry, 1) != -1)
            FindOrAllocateLink(Core::Hook::CallMethod<int32>(0xBE6200, entry, this, 1), true);
    }

    Core::Hook::CallMethod<void>(0x8ADE20, self + 0x1130, this);
    Core::Hook::CallMethod<void>(0x842290, self + 0x12E0, this);

    m_nVehFlagsF6F ^= (m_nVehFlagsF6F ^ (static_cast<uint8>(IsSpecialVehicleModel(modelIndex)) << 6)) & 0x40u;
    m_nVehFlagsF6F ^= (m_nVehFlagsF6F ^ (static_cast<uint8>(IsSpecialModelIndex(modelIndex)) << 5)) & 0x20u;

    bool bCond = Core::StaticRef<int32>(0x10F8044) >= 2 &&
                 (m_nModelIndex == Core::StaticRef<int32>(0x1230478) || m_nModelIndex == Core::StaticRef<int32>(0x12306B8));
    m_nVehFlagsF67 ^= (m_nVehFlagsF67 ^ (static_cast<uint8>(bCond) << 4)) & 0x10u;
    m_nVehFlagsF71 ^= (m_nVehFlagsF71 ^ (static_cast<uint8>(!bCond) << 4)) & 0x10u;

    bool bSpecial = (modelIndex == Core::StaticRef<int32>(0x1230778) ||
                     modelIndex == Core::StaticRef<int32>(0x12308EC) ||
                     modelIndex == Core::StaticRef<int32>(0x1230370) ||
                     modelIndex == Core::StaticRef<int32>(0x1230A0C) ||
                     modelIndex == Core::StaticRef<int32>(0x12302F8) ||
                     (m_nVehFlagsF6F & 0x40u) != 0);
    m_nVehFlagsF6E ^= (m_nVehFlagsF6E ^ (static_cast<uint8>(bSpecial) << 5)) & 0x20u;
    m_nVehFlagsF6E ^= (m_nVehFlagsF6E ^ ((*reinterpret_cast<const int32*>(mi + 0x6C) == 4) ? 0x40 : 0)) & 0x40u;

    reinterpret_cast<void(__thiscall*)(CVehicle*)>(m_vftable[87])(this);
    reinterpret_cast<void(__thiscall*)(CVehicle*)>(m_vftable[90])(this);
    reinterpret_cast<void(__thiscall*)(CVehicle*)>(m_vftable[72])(this);

    m_nVehUnk250 = 1;
    reinterpret_cast<void(__thiscall*)(void*, CVehicle*)>((*reinterpret_cast<void***>(m_audPlaceableTracker))[2])(m_audPlaceableTracker, this);
    Core::Hook::CallMethod<void>(0xAEA300, m_vehSubObj260, this);
    Core::Hook::CallMethod<void>(0xCBB4C0, m_vehAudioEntity, this);
}

__declspec(noinline) static void CallUsercall_9CCBE0(
    const float* pNegAt, const float* pCenter, const float* pUpVec,
    float halfX, float halfY, float halfZ, float intensity, int32 flag)
{
    uintptr_t fn = Core::Rebase(0x9CCBE0);
    __asm
    {
        push flag
        push intensity
        push halfZ
        push halfY
        push halfX
        push pUpVec
        push pCenter
        mov eax, pNegAt
        call fn
        add esp, 1Ch
    }
}

void __thiscall CVehicle::ProcessLightsAndEffects(float* pMat, uint32 flags)
{
    uint8* self = reinterpret_cast<uint8*>(this);
    if (Core::StaticRef<uint8>(0x161924A) != 0 || (m_nUnk08 & Core::StaticRef<uint32>(0x1135BA4)) == 0)
        return;

    auto clampVal = [](float v, float lo, float hi) -> float
    {
        return (v < lo) ? lo : ((v > hi) ? hi : v);
    };

    uint8 bBlink = (CTimer::ms_nTimeMs < m_nVehUnkF7C && (CTimer::ms_nTimeMs & 0x100u) != 0) ? 1 : 0;
    bBlink |= (m_nVehFlagsF69 >> 1) & 1u;

    float fCamDistSq = static_cast<float>(Core::Hook::Call<double>(0xB1E4C0, &m_pMatrix->pos));
    float fZDiff = m_pMatrix->pos.z - *reinterpret_cast<const float*>(Core::StaticRef<const uint8*>(0x10F47F0) + 0x88);
    float fZScale = (fZDiff > 0.0f) ? clampVal(2.0f - fZDiff, 0.0f, 1.0f) : 1.0f;
    float fCamDist = (fZScale <= 0.0f && fZDiff > 0.0f) ? 0.0f : std::sqrtf(fCamDistSq);

    float fS = Core::StaticRef<float>(0xF33A9C);
    float v161 = clampVal((80.0f - fCamDist) * fS, 0.0f, 1.0f) * fZScale;
    float v150 = clampVal((40.0f - fCamDist) * fS, 0.0f, 1.0f) * fZScale;
    float v166 = clampVal((150.0f - fCamDist) * fS, 0.0f, 1.0f) * fZScale;
    float v13 = clampVal((290.0f - fCamDist) * 0.1f, 0.0f, 1.0f);
    float v14 = clampVal((fCamDist - 50.0f) * 0.003000000026077032f, 0.0f, 0.75f);
    float v158 = (v14 + Core::StaticRef<float>(0xD82360)) * v13;

    // Headlight auto-update matches 0x9D0F40..0x9D0FE1
    bool bWantAutoHead = ShouldSkipLodOrUpdate();
    uint8 curHeadBit = m_nVehFlagsF65 & 1u;
    if (static_cast<uint8>(bWantAutoHead ? 1 : 0) != curHeadBit)
    {
        uint32 entityType = (m_nFlags2 >> 10) & 0x1Fu;
        if (entityType != 3u)
        {
            if (entityType == 2u && GetVehicleType() != 3)
            {
                if (curHeadBit != 0 && m_pMatrix)
                {
                    float dx = std::fabsf(Core::StaticRef<float>(0x11DC4E0) - m_pMatrix->pos.x);
                    float dy = std::fabsf(Core::StaticRef<float>(0x11DC4E4) - m_pMatrix->pos.y);
                    if (dx + dy > Core::StaticRef<float>(0xD8F340))
                        m_nVehFlagsF65 &= ~1u;
                }
            }
            else
            {
                if (bWantAutoHead)
                    m_nVehFlagsF65 |= 1u;
                else
                    m_nVehFlagsF65 &= ~1u;
            }
        }
    }

    bool bForce = false;
    uint8 mode1112 = m_nVehUnk1112 & 3u;
    if (mode1112 == 1)
        m_nVehFlagsF65 &= ~1u;
    else if (mode1112 == 2)
        bForce = true;

    if (m_pDrawHandler)
    {
        uint8* p4 = *reinterpret_cast<uint8**>(static_cast<uint8*>(m_pDrawHandler) + 4);
        if (p4)
            std::memcpy(p4 + 0x90, reinterpret_cast<const void*>(Core::Rebase(0x1619204)), 0x3C);
    }

    CVehicleModelInfo* mi = static_cast<CVehicleModelInfo*>(CModelInfo::ms_modelInfoPtrs[m_nModelIndex]);
    if ((m_nFlags2 & 0x7C00u) != 0xC00u &&
        v161 > 0.01f &&
        (Core::StaticRef<uint8>(0x1619344) == 1 || (mi->m_nVehicleFlags & 0x800u) == 0))
    {
        alignas(16) float negAt[4] = { -pMat[8], -pMat[9], -pMat[10], 0.0f };
        alignas(16) float upVec[4] = { pMat[4], pMat[5], pMat[6], 0.0f };
        uint8* pSkel = Core::Hook::CallMethod<uint8*>(0x447A20, this);
        if (GetVehicleType() == 1)
        {
            const int32* pCC = reinterpret_cast<const int32*>(mi->m_pVehicleStruct);
            int32 b1 = pCC[0x24 / 4];
            int32 b2 = pCC[0x34 / 4];
            if (b1 > -1 && b2 > -1 && pSkel)
            {
                const uint8* localMats = *reinterpret_cast<const uint8* const*>(pSkel + 0x14);
                const float* pMatA = reinterpret_cast<const float*>(localMats + (b1 << 6));
                const float* pMatB = reinterpret_cast<const float*>(localMats + (b2 << 6));
                float h = *reinterpret_cast<const float*>(reinterpret_cast<const uint8*>(mi) + 0x74) * Core::StaticRef<float>(0xF33ADC);
                CVector wFront(pMatA[12], pMatA[13], pMatA[14]);
                CVector wRear(pMatB[12], pMatB[13], pMatB[14]);
                float blend = Core::StaticRef<float>(0xF33AE0);
                CVector mid = wFront * blend + wRear * (1.0f - blend);
                if ((Core::StaticRef<uint32>(0x1619340) & 1u) == 0)
                {
                    Core::StaticRef<uint32>(0x1619340) |= 1u;
                    Core::StaticRef<float>(0x1619330) = 0.0f;
                    Core::StaticRef<float>(0x1619334) = 0.0f;
                    Core::StaticRef<float>(0x1619338) = 0.0f;
                }
                alignas(16) float center[4] = {
                    mid.x + Core::StaticRef<float>(0x1619330),
                    mid.y + Core::StaticRef<float>(0x1619334),
                    mid.z + Core::StaticRef<float>(0x1619338),
                    0.0f
                };
                CVector diff = wFront - wRear;
                float lenDiff = std::sqrtf(diff.MagnitudeSquared());
                float halfX = (h * Core::StaticRef<float>(0xF33AD0)) * 0.5f;
                float halfY = ((lenDiff + h * 2.0f) * Core::StaticRef<float>(0xF33ACC)) * 0.5f;
                float halfZ = (h * Core::StaticRef<float>(0xF33AD4)) * 0.5f;
                CallUsercall_9CCBE0(negAt, center, upVec,
                                    halfX, halfY, halfZ,
                                    v161, Core::StaticRef<int32>(0xF33AD8));
            }
        }
        else if (pSkel)
        {
            const int32 slots[4] = { 9, 10, 13, 14 };
            const uint8* localMats = *reinterpret_cast<const uint8* const*>(pSkel + 0x14);
            const uint8* objMats = *reinterpret_cast<const uint8* const*>(pSkel + 0x0C);
            CVector worldPts[4];
            CVector objPts[4];
            int32 count = 0;
            for (int32 i = 0; i < 4; ++i)
            {
                int32 boneId = static_cast<int32>(mi->m_pVehicleStruct->m_nBones[slots[i]]);
                if (boneId > -1)
                {
                    const float* pmWorld = reinterpret_cast<const float*>(localMats + (boneId << 6));
                    const float* pmObj = reinterpret_cast<const float*>(objMats + boneId * 0x50);
                    worldPts[count] = { pmWorld[12], pmWorld[13], pmWorld[14] };
                    objPts[count] = { pmObj[12], pmObj[13], pmObj[14] };
                    ++count;
                }
            }
            float halfZ = *reinterpret_cast<const float*>(reinterpret_cast<const uint8*>(mi) + 0x74) * 2.0f * 0.5f;
            if (count == 4)
            {
                CVector c = (worldPts[0] + worldPts[1] + worldPts[2] + worldPts[3]) * 0.25f;
                alignas(16) float center[4] = { c.x, c.y, c.z, 0.0f };
                float halfX = (std::sqrtf((worldPts[1] - worldPts[0]).MagnitudeSquared()) +
                               std::sqrtf((worldPts[2] - worldPts[3]).MagnitudeSquared())) * 0.5f * 0.5f;
                float halfY = (std::sqrtf((worldPts[0] - worldPts[2]).MagnitudeSquared()) +
                               std::sqrtf((worldPts[1] - worldPts[3]).MagnitudeSquared())) * 0.5f * 0.5f;
                CallUsercall_9CCBE0(negAt, center, upVec, halfX, halfY, halfZ, v161, 0);
            }
            else if (count == 3)
            {
                float minX = Core::StaticRef<float>(0xD69830), maxX = Core::StaticRef<float>(0xD6982C);
                float minY = minX, maxY = maxX;
                float sumZ = 0.0f;
                for (int32 i = 0; i < 3; ++i)
                {
                    if (objPts[i].x < minX) minX = objPts[i].x;
                    if (objPts[i].y < minY) minY = objPts[i].y;
                    if (objPts[i].x > maxX) maxX = objPts[i].x;
                    if (objPts[i].y > maxY) maxY = objPts[i].y;
                    sumZ += objPts[i].z;
                }
                CVector localC(
                    (maxX - minX) * 0.5f + minX,
                    (maxY - minY) * 0.5f + minY,
                    sumZ * Core::StaticRef<float>(0xEBE2A0));
                alignas(16) float center[4] = {
                    pMat[0] * localC.x + pMat[4] * localC.y + pMat[8]  * localC.z + pMat[12],
                    pMat[1] * localC.x + pMat[5] * localC.y + pMat[9]  * localC.z + pMat[13],
                    pMat[2] * localC.x + pMat[6] * localC.y + pMat[10] * localC.z + pMat[14],
                    0.0f
                };
                CallUsercall_9CCBE0(negAt, center, upVec, (maxX - minX) * 0.5f, (maxY - minY) * 0.5f, halfZ, v161, 0);
            }
        }
    }

    uint8 blink = static_cast<uint8>(((static_cast<uint32>(static_cast<uint16>(m_nRandomSeed)) + CTimer::ms_nTimeMs) >> 9) & 1u);
    uint16 f98 = static_cast<uint16>(m_nVehUnkF98);
    bool bHazardSiren = (f98 != 0 && f98 != 0xFFFFu && (m_nFlags2 & 0x7C00u) != 0xC00u);
    uint8 bEngineOn = (m_nVehFlagsF64 >> 3) & 1u;

    uint8 bIndL = (bEngineOn && (m_nVehFlagsF6C & 1u) != 0) ? 1 : 0;
    uint8 bIndR = (bEngineOn && (m_nVehFlagsF6B & 0x80u) != 0) ? 1 : 0;
    uint8 bSirenBlink = bForce ? 1 : 0;

    if (m_nVehFlagsF6C & 2u)
    {
        bIndL = blink;
        bIndR = blink;
    }
    else if (bHazardSiren)
    {
        bSirenBlink = blink;
        bIndL = blink;
        bIndR = blink;
    }
    else if (m_nVehFlagsF6C & 4u)
    {
        bIndL = 1;
        bIndR = 1;
    }

    uint8 bFrontEn = static_cast<uint8>((~(flags >> 1)) & 1u);
    uint8 bRearEn  = static_cast<uint8>((~(flags >> 2)) & 1u);
    uint8 bCheckDmg = static_cast<uint8>((~flags) & 1u);

    if (bEngineOn || bSirenBlink != bEngineOn)
    {
        bool bBraking = static_cast<double>(m_fVehUnk10C8) < Core::StaticRef<double>(0xD82358);
        uint8 bHandbrakeOrParked = m_nVehFlagsF64 >> 7;
        uint8 bRevFlag = 0;

        CPed* pDriver = m_pDriver;
        if (pDriver && pDriver->m_nPedUnk218 == 0 && pDriver->m_nPedUnk219 != 0 && !Core::Hook::Call<bool>(0x444370))
        {
            uint8* pCtrl = Core::Hook::CallMethod<uint8*>(0x9BE5D0, pDriver);
            if (pCtrl && static_cast<uint8>(pCtrl[0x291E] ^ pCtrl[0x291C]) > 0x10u &&
                static_cast<uint8>(pCtrl[0x292E] ^ pCtrl[0x292C]) > 0x10u)
            {
                bRevFlag = 1;
            }
            bHandbrakeOrParked = 0;
        }

        bool bRev = ((m_fVehUnk10CC > 0.1f || bRevFlag == 1) && !bHandbrakeOrParked && pDriver && (m_nVehFlagsF6C & 8u) == 0);
        uint8 typeEBE = self[0xEBE];
        if (typeEBE == 10 || typeEBE == 11 || typeEBE == 12 ||
            ((typeEBE == 4 || typeEBE == 14) && pDriver && pDriver->m_nDummyConvertState == 2))
        {
            uint8 oldState = (static_cast<uint8>(self[0xFCC]) >> 5) & 1u;
            uint8 cnt = static_cast<uint8>(self[0xFCC]) & 0x1Fu;
            uint8 curRev = bRev ? 1u : 0u;
            if (oldState != curRev)
            {
                if (cnt == 0)
                {
                    oldState = curRev;
                    cnt = 16;
                }
                --cnt;
            }
            else if (cnt != 0)
            {
                --cnt;
            }
            bRev = (oldState != 0);
            self[0xFCC] = static_cast<uint8>(cnt | (oldState ? 0x20u : 0u));
        }

        bool bWantHead = ((m_nVehFlagsF65 & 1u) != 0 || bSirenBlink != 0);
        bool bHeadFront = (bWantHead && bFrontEn != 0) || (bBlink != 0);

        auto bulbOk = [&](bool enable, uint32 dmgIdx) -> uint8
        {
            return (enable && (!bCheckDmg || m_nVehBoneResetMaskA[dmgIdx] == 0)) ? 1 : 0;
        };

        uint8 bHeadR    = bulbOk(bHeadFront, 1);
        uint8 bHeadL    = bulbOk(bHeadFront, 0);
        uint8 bTailR    = bulbOk(bWantHead && bRearEn != 0, 3);
        uint8 bRevR     = bulbOk(bRev && bRearEn != 0, 9);
        uint8 bBrakeR   = bulbOk(bBraking && bRearEn != 0, 12);
        uint8 bTailL    = bulbOk(bWantHead && bRearEn != 0, 2);
        uint8 bRevL     = bulbOk(bRev && bRearEn != 0, 8);
        uint8 bBrakeL   = bulbOk(bBraking && bRearEn != 0, 11);
        uint8 bRevExtra = bulbOk(bRev && bRearEn != 0, 10);

        Core::Hook::CallMethod<void>(0x9CD240, this, 50, 51, bHeadL, bHeadR, v161, v158, mi, pMat, bBlink);
        Core::Hook::CallMethod<void>(0x9CDAF0, this, 52, -1, 53, bTailL, 0, bTailR, v161, v158, mi, pMat, 0);
        Core::Hook::CallMethod<void>(0x9CDAF0, this, 58, 60, 59, bRevL, bRevExtra, bRevR, v150, v158, mi, pMat, 1);
        Core::Hook::CallMethod<void>(0x9CE2F0, this, 61, 62, bBrakeL, bBrakeR, v150, mi, pMat);

        if ((m_nVehFlagsF64 & 8u) != 0 && m_pDrawHandler)
        {
            uint8* p4 = *reinterpret_cast<uint8**>(static_cast<uint8*>(m_pDrawHandler) + 4);
            if (p4)
                *reinterpret_cast<uint32*>(p4 + 0x90) = Core::StaticRef<uint32>(0x16191C8);
        }
    }

    auto indOk = [&](bool en, uint32 dmgIdx) -> bool
    {
        return en && (!bCheckDmg || m_nVehBoneResetMaskA[dmgIdx] == 0);
    };
    bool indFR = indOk(bIndR && bFrontEn, 4);
    bool indFL = indOk(bIndL && bFrontEn, 5);
    bool indRR = indOk(bIndR && bRearEn, 6);
    bool indRL = indOk(bIndL && bRearEn, 7);

    if (indFR) Core::Hook::CallMethod<void>(0x9CD0F0, this, 54, v150, mi, pMat);
    if (indFL) Core::Hook::CallMethod<void>(0x9CD0F0, this, 55, v150, mi, pMat);
    if (indRR) Core::Hook::CallMethod<void>(0x9CD0F0, this, 56, v150, mi, pMat);
    if (indRL) Core::Hook::CallMethod<void>(0x9CD0F0, this, 57, v150, mi, pMat);

    Core::Hook::CallMethod<void>(0xAEAC00, m_vehSubObj260, (indFL || indFR || indRL || indRR) ? 1 : 0);

    if (m_nVehFlagsF6E & 0x20u)
    {
        uint32 arg0 = ((m_nVehFlagsF69 & 0x10u) != 0 && (m_nVehFlagsF69 & 0x20u) == 0) ? 1u : 0u;
        Core::Hook::CallMethod<void>(0x9D0880, this, arg0, v166, mi);
    }

    if (Core::StaticRef<int32>(0x10F8044) >= 2 &&
        (m_nModelIndex == Core::StaticRef<int32>(0x1230478) || m_nModelIndex == Core::StaticRef<int32>(0x12306B8)))
    {
        Core::Hook::CallMethod<void>(0x9D0320, this, (m_nVehFlagsF64 >> 3) & 1u, v166, mi);
    }

    uint8 sirenFlag = (m_nVehFlagsF6C >> 4) & 1u;
    int32 cntFDC = static_cast<int32>(m_nVehUnkFDC);
    for (int32 i = 0; i < cntFDC; ++i)
    {
        const uint8* e = reinterpret_cast<const uint8*>(m_nVehUnkFD8) + i * 0x34;
        int32 eType = *reinterpret_cast<const int32*>(e);
        if (eType != 0x27 && eType != 0x28 &&
            (*reinterpret_cast<const float*>(e + 0x1C) >= 0.01f || !Core::Hook::CallMethod<bool>(0xA5E400, e, 0)))
        {
            sirenFlag |= 1u;
            break;
        }
    }
    if (sirenFlag)
        Core::Hook::CallMethod<void>(0x9D09E0, this, 0x40, v150, mi);

    if (GetVehicleType() == 0 && (self[0x14C4] & 1u) != 0 && self[0x11ED] == 0)
        Core::Hook::CallMethod<void>(0x9D0BF0, this, v150, mi);

    if (*reinterpret_cast<const uint8*>(reinterpret_cast<const uint8*>(mi) + 0x3C0) == 1)
        Core::Hook::CallMethod<void>(0xCBBED0, self + 0x12E0);
}

void __thiscall CVehicle::ApplyModelDoorRotation(int32 doorSlot, float* pMat3x4, uint8* pOutFlag)
{
    CVehicleModelInfo* mi = static_cast<CVehicleModelInfo*>(CModelInfo::ms_modelInfoPtrs[m_nModelIndex]);
    int32 handlingId = *reinterpret_cast<const int32*>(reinterpret_cast<const uint8*>(mi) + 0xC4);

    if (mi->m_nVehicleFlags & 0x20u)
    {
        if (doorSlot == 2)
        {
            pMat3x4[0] = 0.0f;  pMat3x4[1] = -1.0f; pMat3x4[2] = 0.0f;
            pMat3x4[4] = 1.0f;  pMat3x4[5] = 0.0f;  pMat3x4[6] = 0.0f;
            pMat3x4[8] = 0.0f;  pMat3x4[9] = 0.0f;  pMat3x4[10] = 1.0f;
            *pOutFlag = 1;
        }
        else if (doorSlot == 4)
        {
            pMat3x4[0] = 0.0f;  pMat3x4[1] = 1.0f;  pMat3x4[2] = 0.0f;
            pMat3x4[4] = -1.0f; pMat3x4[5] = 0.0f;  pMat3x4[6] = 0.0f;
            pMat3x4[8] = 0.0f;  pMat3x4[9] = 0.0f;  pMat3x4[10] = 1.0f;
            *pOutFlag = 1;
        }
    }
    else if (GetVehicleType() == 4 && handlingId == 0x59)
    {
        if (doorSlot == 2)
        {
            pMat3x4[0] = 0.0f;  pMat3x4[1] = 1.0f;  pMat3x4[2] = 0.0f;
            pMat3x4[4] = -1.0f; pMat3x4[5] = 0.0f;  pMat3x4[6] = 0.0f;
            pMat3x4[8] = 0.0f;  pMat3x4[9] = 0.0f;  pMat3x4[10] = 1.0f;
        }
        else if (doorSlot == 4)
        {
            pMat3x4[0] = 0.0f;  pMat3x4[1] = -1.0f; pMat3x4[2] = 0.0f;
            pMat3x4[4] = 1.0f;  pMat3x4[5] = 0.0f;  pMat3x4[6] = 0.0f;
            pMat3x4[8] = 0.0f;  pMat3x4[9] = 0.0f;  pMat3x4[10] = 1.0f;
        }
    }

    if (GetVehicleType() == 3)
    {
        if (doorSlot == 2 || doorSlot == 1)
        {
            Core::Hook::CallMethod<void>(0x425050, pMat3x4, -1.5707963705062866f);
            pMat3x4[0] = 0.0f;  pMat3x4[1] = -1.0f; pMat3x4[2] = 0.0f;
            pMat3x4[4] = 1.0f;  pMat3x4[5] = 0.0f;  pMat3x4[6] = 0.0f;
            pMat3x4[8] = 0.0f;  pMat3x4[9] = 0.0f;  pMat3x4[10] = 1.0f;
            *pOutFlag = 1;
        }
        else if (doorSlot == 4 || doorSlot == 3)
        {
            Core::Hook::CallMethod<void>(0x425050, pMat3x4, 1.5707963705062866f);
            pMat3x4[0] = 0.0f;  pMat3x4[1] = 1.0f;  pMat3x4[2] = 0.0f;
            pMat3x4[4] = -1.0f; pMat3x4[5] = 0.0f;  pMat3x4[6] = 0.0f;
            pMat3x4[8] = 0.0f;  pMat3x4[9] = 0.0f;  pMat3x4[10] = 1.0f;
            *pOutFlag = 1;
        }
    }
}

bool __thiscall CVehicle::IsClearAbove(const float* pPos)
{
    uint32 v5[24] = {};
    const CVector& defV = Core::StaticRef<CVector>(0x18A8C50);
    *reinterpret_cast<CVector*>(&v5[4]) = defV;
    *reinterpret_cast<CVector*>(&v5[8]) = defV;
    *reinterpret_cast<CVector*>(&v5[12]) = defV;
    v5[19] = 0xFFFFFFFFu;
    float fZOff = (GetVehicleType() == 4) ? Core::StaticRef<float>(0xD7817C) : Core::StaticRef<float>(0xD8200C);
    return !Core::Hook::Call<bool>(0x991E90, pPos, pPos[2] - fZOff, 0, v5, 6, 4);
}

void __thiscall CVehicle::UpdateInteriorModelStream()
{
    uint8* mi = reinterpret_cast<uint8*>(CModelInfo::ms_modelInfoPtrs[m_nModelIndex]);
    int32 id = *reinterpret_cast<const int32*>(mi + 0x34C);
    if (id == -1)
        return;

    const CVector& refPos = Core::StaticRef<CVector>(0x11DC4E0);
    CVector d = m_pMatrix->pos - refPos;
    float distSq = d.MagnitudeSquared();
    float dist = std::sqrtf(distSq);

    void* pPlayerVeh = Core::Hook::Call<void*>(0x817F80, 0);
    bool cond = (m_nUnk63 > 0 && dist < Core::StaticRef<float>(0xEBE000)) ||
                (this == pPlayerVeh) ||
                (Core::Hook::Call<bool>(0x405180) && Core::Hook::Call<double>(0x799240) > distSq);

    if (cond && (m_nVehFlagsF70 & 0x40u) == 0)
    {
        if (Core::Hook::Call<bool>(0x832DD0, id, CModelInfo::ms_nStreamingModuleId))
        {
            m_nVehFlagsF70 |= 0x40u;
            uint8* mi2 = reinterpret_cast<uint8*>(CModelInfo::ms_modelInfoPtrs[id]);
            Core::Hook::CallMethod<void>(0x98E460, mi2);
            void* p3 = *reinterpret_cast<void**>(*reinterpret_cast<uint8**>(mi2 + 0xC) + 4);
            m_nVehUnkFEC = reinterpret_cast<uint32(__thiscall*)(void*, CVehicle*)>((*reinterpret_cast<void***>(p3))[5])(p3, this);
            void* pDrawObj = m_pDrawHandler ? *reinterpret_cast<void**>(static_cast<uint8*>(m_pDrawHandler) + 4) : nullptr;
            Core::Hook::CallMethod<void>(0xCBAAB0, reinterpret_cast<void*>(m_nVehUnkFEC), pDrawObj, this, id);
        }
        else
        {
            Core::Hook::Call<void>(0x832C40, id, CModelInfo::ms_nStreamingModuleId, 0);
        }
    }
    else if (!cond && (m_nVehFlagsF70 & 0x40u) != 0)
    {
        m_nVehFlagsF70 &= ~0x40u;
        if (Core::Hook::Call<bool>(0x832DD0, id, CModelInfo::ms_nStreamingModuleId))
        {
            Core::Hook::CallMethod<void>(0x98E470, CModelInfo::ms_modelInfoPtrs[id]);
            void* pFEC = reinterpret_cast<void*>(m_nVehUnkFEC);
            if (pFEC)
                reinterpret_cast<void(__thiscall*)(void*, int32)>((*reinterpret_cast<void***>(pFEC))[0])(pFEC, 1);
            m_nVehUnkFEC = 0;
        }
    }
}

bool __thiscall CVehicle::GetBoneEntryUnkC299A0(int32 boneSlot, float* pOutVec3)
{
    pOutVec3[0] = pOutVec3[1] = pOutVec3[2] = 0.0f;
    CVehicleModelInfo* mi = static_cast<CVehicleModelInfo*>(CModelInfo::ms_modelInfoPtrs[m_nModelIndex]);
    int32 id = static_cast<int32>(mi->m_pVehicleStruct->m_nBones[boneSlot]);
    if (id == -1)
        return false;
    Core::Hook::CallMethod<void>(0xC299A0, this, id, pOutVec3);
    return true;
}

bool __thiscall CVehicle::GetBoneEntryUnk9C8900(int32 boneSlot, void* pOut)
{
    CVehicleModelInfo* mi = static_cast<CVehicleModelInfo*>(CModelInfo::ms_modelInfoPtrs[m_nModelIndex]);
    int32 id = static_cast<int32>(mi->m_pVehicleStruct->m_nBones[boneSlot]);
    if (id == -1)
        return false;
    GetBoneMatrix(id, static_cast<float*>(pOut));
    return true;
}

void __thiscall CVehicle::RegisterInUnk18A82AC(int32)
{
    Core::Hook::CallMethod<void>(0x818180, Core::StaticRef<void*>(0x18A82AC), reinterpret_cast<void*>(Core::Rebase(0xCFAEE0)), this);
}

double __thiscall CVehicle::ComputeSeatEntryTransform(CMatrix* outMat, CEntity* pPed, int32 doorIdx, int32 bCheckBlocked,
                                                      float fProgress, float fBlend, float fOffsetScale, const CVector* pLocalOff,
                                                      float fHeadingOffset, bool bForceSide, bool bFlipSide, bool bDoProbe)
{
    uint8* self = reinterpret_cast<uint8*>(this);
    CVehicleModelInfo* mi = static_cast<CVehicleModelInfo*>(CModelInfo::ms_modelInfoPtrs[m_nModelIndex]);

    CMatrix localMat;
    localMat.right = { 1.0f, 0.0f, 0.0f };
    localMat.up = { 0.0f, 1.0f, 0.0f };
    localMat.at = { 0.0f, 0.0f, 1.0f };
    localMat.pos = { 0.0f, 0.0f, 0.0f };

    int32 boneId = static_cast<int32>(mi->m_pVehicleStruct->m_nBones[doorIdx]);
    if (boneId != -1)
        localMat = *static_cast<const CMatrix*>(GetBoneLocalMatrix(boneId));

    uint8 unusedFlag = 0;
    ApplyModelDoorRotation(doorIdx, &localMat.right.x, &unusedFlag);

    if (GetVehicleType() == 2)
        bForceSide = true;

    CVector off(0.0f, 0.0f, 0.0f);
    float latPush = 0.0f;
    CVector worldOff(0.0f, 0.0f, 0.0f);

    if (fProgress <= 1.0f)
    {
        if (bCheckBlocked != 0)
        {
            float v133[4] = {};
            GetUnkSeatOffset(v133, bCheckBlocked, reinterpret_cast<int32>(pPed));
            off = { v133[0], v133[1], v133[2] };

            float rightZ = m_pMatrix->right.z;
            float handOff = *reinterpret_cast<const float*>(*reinterpret_cast<const uint8* const*>(self + 0xE18) + 0xE4);
            float absRightZ = std::fabsf(rightZ);

            if (GetVehicleType() == 1)
            {
                worldOff.z = off.z;
                float h = m_pMatrix
                    ? static_cast<float>(std::atan2(static_cast<double>(-m_pMatrix->up.x), static_cast<double>(m_pMatrix->up.y)))
                    : m_fPad1C;
                float s = std::sinf(h), c = std::cosf(h);
                worldOff.x = (c * off.x - off.y * s) * (1.0f - fOffsetScale);
                worldOff.y = (off.y * c + s * off.x) * (1.0f - fOffsetScale);
                off = off * fOffsetScale;

                float rz = 1.0f - std::fabsf(m_pMatrix->at.z);
                if (rz < 0.0f) rz = 0.0f;
                if (rz > 1.0f) rz = 1.0f;
                worldOff.z = rz * localMat.pos.z + (1.0f - fOffsetScale) * worldOff.z;
                latPush = 0.0f;
            }
            else
            {
                latPush = (1.0f - fOffsetScale) * absRightZ * Core::StaticRef<float>(0xEBE218);
            }

            if (off.x < 0.0f)
            {
                latPush = -latPush;
                handOff = -handOff;
            }
            if (bForceSide)
                latPush = 0.0f;

            off.x += (1.0f - fProgress) * handOff;
        }
    }
    else
    {
        int32 altIdx = (doorIdx == 1 || doorIdx == 2) ? (doorIdx + 2) : (doorIdx - 2);
        int32 altBone = static_cast<int32>(mi->m_pVehicleStruct->m_nBones[altIdx]);
        if (altBone != -1)
        {
            const CMatrix* altMat = static_cast<const CMatrix*>(GetBoneLocalMatrix(altBone));
            off = (altMat->pos - localMat.pos) * (fProgress - 1.0f);
        }
    }

    float ang = static_cast<float>(std::atan2(static_cast<double>(-localMat.up.x), static_cast<double>(localMat.up.y)));
    float sL = std::sinf(ang), cL = std::cosf(ang);
    localMat.pos.x += cL * off.x - off.y * sL;
    localMat.pos.y += off.y * cL + sL * off.x;
    localMat.pos.z += off.z;

    CMatrix vehMat = *m_pMatrix;
    if (bFlipSide)
    {
        vehMat.right.x = -vehMat.right.x;
        vehMat.right.y = -vehMat.right.y;
        vehMat.right.z = -vehMat.right.z;
        vehMat.at.x = -vehMat.at.x;
        vehMat.at.y = -vehMat.at.y;
        vehMat.at.z = -vehMat.at.z;
        localMat.pos.x = -localMat.pos.x;
        localMat.pos.z = -localMat.pos.z;
    }

    if (GetVehicleType() == 1 && doorIdx == 2 && static_cast<int32>(mi->m_pVehicleStruct->m_nBones[23]) == -1)
    {
        if (Core::Hook::CallMethod<void*>(0x447A20, this))
        {
            uint8* w9 = static_cast<uint8*>(FindSubObjFD0(9));
            uint8* w13 = static_cast<uint8*>(FindSubObjFD0(13));
            const uint8* skel = *Core::Hook::CallMethod<const uint8* const*>(0x447AE0, this);
            const CVector& bone13Pos = *reinterpret_cast<const CVector*>(skel + static_cast<int32>(mi->m_pVehicleStruct->m_nBones[13]) * 0xE0 + 0x20);
            float r9 = *reinterpret_cast<const float*>(w9 + 0x64);
            Core::Hook::CallMethod<double>(0xB48F60, w9);
            float y = static_cast<float>(Core::Hook::CallMethod<double>(0xB48F60, w13)) - *reinterpret_cast<const float*>(w13 + 0x20);
            float x = r9 - bone13Pos.y;
            float a = static_cast<float>(std::atan2(static_cast<double>(y), static_cast<double>(x))) * Core::StaticRef<float>(0xF33AEC);
            Core::Hook::CallMethod<void>(0x6ABEF0, &vehMat, -a);
            vehMat.pos = vehMat.pos + vehMat.at * (std::sinf(a) * r9);
        }
    }

    if (pLocalOff)
    {
        localMat.pos.x += localMat.right.x * pLocalOff->x + localMat.up.x * pLocalOff->y + localMat.at.x * pLocalOff->z;
        localMat.pos.y += localMat.right.y * pLocalOff->x + localMat.up.y * pLocalOff->y + localMat.at.y * pLocalOff->z;
        localMat.pos.z += localMat.right.z * pLocalOff->x + localMat.up.z * pLocalOff->y + localMat.at.z * pLocalOff->z;
    }

    auto transformVec = [&](const CVector& v) -> CVector
    {
        return {
            v.x * vehMat.right.x + v.y * vehMat.up.x + v.z * vehMat.at.x,
            v.x * vehMat.right.y + v.y * vehMat.up.y + v.z * vehMat.at.y,
            v.x * vehMat.right.z + v.y * vehMat.up.z + v.z * vehMat.at.z
        };
    };

    outMat->right = transformVec(localMat.right);
    outMat->up    = transformVec(localMat.up);
    outMat->at    = transformVec(localMat.at);
    outMat->pos   = transformVec(localMat.pos) + vehMat.pos;

    float xySq = vehMat.right.x * vehMat.right.x + vehMat.right.y * vehMat.right.y;
    float invLen = (xySq == 0.0f) ? 0.0f : (1.0f / std::sqrtf(xySq));
    outMat->pos.x += (invLen * vehMat.right.x) * latPush + worldOff.x;
    outMat->pos.y += (invLen * vehMat.right.y) * latPush + worldOff.y;
    outMat->pos.z += worldOff.z;

    if (fHeadingOffset != 0.0f)
    {
        float ch = std::cosf(fHeadingOffset), sh = std::sinf(fHeadingOffset);
        CVector newRight = outMat->right * ch + outMat->up * sh;
        outMat->up = outMat->up * ch - outMat->right * sh;
        outMat->right = newRight;
    }

    float zStart = outMat->pos.z;
    float zSel = zStart;
    uint8 hitFlag = 0;
    if (fBlend < 1.0f)
    {
        float zTarget = m_pMatrix->pos.z - static_cast<float>(reinterpret_cast<double(__thiscall*)(CVehicle*)>(m_vftable[105])(this)) + 1.0f;
        if (Core::StaticRef<int32>(0x10F8044) >= 2 && m_nModelIndex == static_cast<int16>(Core::StaticRef<int32>(0x123061C)))
        {
            if (pPed && zTarget > pPed->m_pMatrix->pos.z && !Core::Hook::CallMethod<bool>(0x4969E0, pPed))
                zTarget = m_pMatrix->pos.z;
        }
        else if (pPed && zTarget > pPed->m_pMatrix->pos.z && ((*reinterpret_cast<const uint8*>(reinterpret_cast<const uint8*>(pPed) + 0x26C) & 4u) == 0))
        {
            zTarget = m_pMatrix->pos.z;
        }
        if (bForceSide)
            zTarget -= 1.0f;

        if (fProgress > 0.0f)
        {
            CMatrix recMat;
            ComputeSeatEntryTransform(&recMat, pPed, doorIdx, bCheckBlocked, 0.0f, 0.0f, 0.0f, pLocalOff, fHeadingOffset, bForceSide, bFlipSide, bDoProbe);
            zSel = recMat.pos.z;
        }
        else if (bDoProbe)
        {
            CVector probeStart(outMat->pos.x, outMat->pos.y, outMat->pos.z + (GetVehicleType() == 2 ? 0.0f : 1.0f));
            CVector probeEnd = probeStart - Core::StaticRef<CVector>(0x108A330) * 2.5f;
            float probeBuf[4] = { probeStart.x, probeStart.y, probeStart.z, 0.0f };
            bool probeHit = Core::Hook::Call<bool>(0xC8A150, probeBuf, this, 0, 0, 100.0f, 14, &hitFlag);
            zSel = probeHit ? probeBuf[2] : zStart;

            if (bForceSide)
            {
                uint8* bound = reinterpret_cast<uint8*(__thiscall*)(CVehicle*)>(m_vftable[40])(this);
                if (bound)
                {
                    uint8 seg[32] = {};
                    uint8 localSeg[64] = {};
                    Core::Hook::CallMethod<void>(0x534930, seg, &probeStart, &probeEnd);
                    Core::Hook::CallMethod<void>(0x684970, localSeg, seg, bound + 0x10);
                    uint8* ph = *reinterpret_cast<uint8**>(*reinterpret_cast<uint8**>(bound + 4) + 0xC);
                    if (ph[4] == 0xC)
                        ph = **reinterpret_cast<uint8***>(ph + 0x80);
                    float isect[24] = {};
                    Core::Hook::CallMethod<void>(0x425360, isect);
                    if (reinterpret_cast<bool(__thiscall*)(uint8*, void*, void*, float, int32)>((*reinterpret_cast<void***>(ph))[29])(ph, localSeg, isect, 2.0f, -1))
                    {
                        Core::Hook::CallMethod<void>(0x6880F0, isect, bound + 0x10);
                        float f = isect[6] + 1.0f;
                        if (!probeHit || f <= probeBuf[2])
                            zSel = f;
                    }
                }
            }
        }

        float zFinal = zSel;
        if (pPed)
        {
            bool isBoat = (GetVehicleType() == 3);
            float thresh = hitFlag ? Core::StaticRef<float>(0xDCE03C) : Core::StaticRef<float>(0xDCAFF0);
            uint32& ped2A0 = *reinterpret_cast<uint32*>(reinterpret_cast<uint8*>(pPed) + 0x2A0);
            if (zTarget - 1.0f > zSel)
            {
                ped2A0 |= 1u;
                zFinal = zStart;
            }
            else if (isBoat)
            {
                float zBoat = m_pMatrix->pos.z;
                if (zBoat + Core::StaticRef<float>(0xD82374) > zSel)
                {
                    ped2A0 |= 1u;
                    zFinal = zBoat + Core::StaticRef<float>(0xD82120);
                }
                else
                {
                    ped2A0 &= ~1u;
                    zFinal = zSel;
                }
            }
            else if (zSel > thresh + zStart)
            {
                ped2A0 |= 1u;
                zFinal = zStart;
            }
            else
            {
                ped2A0 &= ~1u;
                zFinal = zSel;
            }
        }
        outMat->pos.z = (1.0f - fBlend) * zFinal + fBlend * outMat->pos.z;
    }

    double heading = std::atan2(static_cast<double>(-outMat->up.x), static_cast<double>(outMat->up.y));
    if (fOffsetScale < 1.0f)
    {
        uint8 q1[16], q2[16];
        Core::Hook::CallMethod<void>(0x506DB0, q1, outMat);

        float cH = std::cosf(static_cast<float>(heading));
        float sH = std::sinf(static_cast<float>(heading));
        CMatrix axisMat;
        axisMat.right = { cH, sH, 0.0f };
        axisMat.up = { -sH, cH, 0.0f };
        axisMat.at = { 0.0f, 0.0f, 1.0f };
        axisMat.pos = outMat->pos;

        Core::Hook::CallMethod<void>(0x506DB0, q2, &axisMat);
        Core::Hook::CallMethod<void>(0x507150, q2, fOffsetScale, q1);
        Core::Hook::CallMethod<void>(0x5BE050, outMat, q2);
    }

    return heading;
}

void __thiscall CVehicle::DetachFragPart(int32 boneSlot, float fImpulse)
{
    if (!IsBoneNonZero(boneSlot))
        return;

    CVehicleModelInfo* mi = static_cast<CVehicleModelInfo*>(CModelInfo::ms_modelInfoPtrs[m_nModelIndex]);
    int32 id = static_cast<int32>(mi->m_pVehicleStruct->m_nBones[boneSlot]);
    int32 part = Core::Hook::Call<int32>(0xA4B480, Core::Hook::CallMethod<void*>(0x51DDD0, m_pFragInstE14), id);
    if (part == -1)
        return;

    CVector dir(0.0f, 0.0f, 1.0f);
    switch (boneSlot)
    {
    case 28: dir = m_pMatrix->up; break;
    case 29: dir = m_pMatrix->up * -1.0f; break;
    case 30:
    case 32: dir = m_pMatrix->right * -1.0f; break;
    case 31:
    case 33: dir = m_pMatrix->right; break;
    default: break;
    }

    Core::Hook::CallMethod<void>(0x8CE2B0, reinterpret_cast<void*>(Core::Rebase(0x12C0400)), this, part, fImpulse, &dir);
}

void __thiscall CVehicle::UpdateNetworkOrPhysicsActive(bool bCheckSub)
{
    if (reinterpret_cast<bool(__thiscall*)(CVehicle*)>(m_vftable[74])(this))
        return;

    if (static_cast<uint32>(GetVehicleType()) > 1u)
        return;

    if (!m_pPhysicsInst)
        return;

    uint16 idx = *reinterpret_cast<const uint16*>(static_cast<const uint8*>(m_pPhysicsInst) + 8);
    if (idx == 0xFFFFu)
        return;

    const uint8* pMgr = Core::StaticRef<const uint8*>(0x1666880);
    if (idx >= *reinterpret_cast<const uint16*>(pMgr + 0x26) ||
        (*reinterpret_cast<const uint8*>(*reinterpret_cast<const uint8* const*>(pMgr + 0x70) + idx * 8 + 4) & 3u) != 0)
    {
        return;
    }

    const float* pPos = &m_pMatrix->pos.x;
    bool bOk = bCheckSub
        ? (Core::Hook::Call<bool>(0x4E33C0, pPos) && Core::Hook::Call<bool>(0x4E3400, pPos) && Core::Hook::Call<bool>(0x4E3440, pPos))
        : Core::Hook::Call<bool>(0xC0A190, pPos);
    if (bOk)
        return;

    if (!reinterpret_cast<bool(__thiscall*)(CVehicle*, int32)>(m_vftable[76])(this, 1))
    {
        if (!Core::Hook::CallMethod<bool>(0x4443C0, this))
            m_nFlags |= 8u;
        reinterpret_cast<void(__thiscall*)(CVehicle*)>(m_vftable[55])(this);
    }
}

void __thiscall CVehicle::ProcessControl()
{
    uint8* self = reinterpret_cast<uint8*>(this);
    UpdateInteriorModelStream();
    ProcessEngineOrBurnout();

    *reinterpret_cast<uint16*>(self + 0xFE0) = 0;
    if (m_pDriver || ((m_nVehFlagsF6D & 0x40u) != 0 && m_bHasPretendOccupants != 0))
    {
        m_nVehFlagsF6D &= ~2u;
        m_nVehUnk1300 = CTimer::ms_nTimeMs;
    }

    bool vt74 = reinterpret_cast<bool(__thiscall*)(CVehicle*)>(m_vftable[74])(this);
    int32 vt = GetVehicleType();
    if (vt74 && (vt == 1 || (vt == 0 && (Core::StaticRef<uint8>(0x11DC82C) == 0 || *reinterpret_cast<const uint32*>(self + 0x1488) != 0))))
    {
        uint8* p6C = reinterpret_cast<uint8*>(m_nUnk6C);
        if (m_nCreatedBy != 2 && (!p6C || p6C[0xE] == 0) && (m_nFlags & 8u) != 0)
        {
            m_nFlags &= ~8u;
            Core::Hook::CallMethod<void>(0xA47660, this);
        }
    }
    else if ((m_nFlags & 8u) != 0 && m_nVehUnk12FC < CTimer::ms_nTimeMs)
    {
        if (m_nCreatedBy != 2 && Core::Hook::Call<bool>(0xC0A190, &m_pMatrix->pos) && !Core::Hook::CallMethod<bool>(0x4443C0, this))
            m_nFlags &= ~8u;
        m_nVehUnk12FC = CTimer::ms_nTimeMs - static_cast<uint32>(static_cast<int32>(static_cast<float>(static_cast<uint16>(Core::Hook::Call<int32>(0xD0D21A))) * 0.000030517578f * -1000.0f)) + 1000u;
    }
    else
    {
        UpdateNetworkOrPhysicsActive(false);
    }

    float tmpVel[4] = {};
    const float* pVel = reinterpret_cast<float*(__thiscall*)(CVehicle*, float*)>(m_vftable[59])(this, tmpVel);
    float lenSq = pVel[0] * pVel[0] + pVel[1] * pVel[1] + pVel[2] * pVel[2];
    if (lenSq > Core::StaticRef<float>(0xF33AA4))
    {
        int32 wheelCnt = static_cast<int32>(m_nVehUnkFD4);
        int32 hitCnt = 0;
        void* pMatMgr = Core::StaticRef<void*>(0x18A87B8);
        for (int32 i = 0; i < wheelCnt; ++i)
        {
            const uint8* w = reinterpret_cast<const uint8*>(m_nVehUnkFD0) + i * 0x170;
            if (w[0x164] & 1u)
            {
                const uint8* matInfo = reinterpret_cast<const uint8*(__thiscall*)(void*, uint32)>((*reinterpret_cast<void***>(pMatMgr))[5])(pMatMgr, *reinterpret_cast<const uint32*>(w + 0xD8));
                if (*reinterpret_cast<const uint16*>(matInfo + 0x20) == 3)
                    ++hitCnt;
            }
        }
        if (hitCnt > 1)
            Core::Hook::Call<void>(0x987730, 22, &m_pMatrix->pos, this, 0, 0, 0.0f);
    }

    if (HasAliveOccupants() && (m_nFlags & 0x8000000u) != 0 && m_nInteriorHash != -1)
    {
        void* pO = Core::Hook::CallMethod<void*>(0xA4C560, Core::StaticRef<void*>(0x165D2E4), m_nInteriorHash);
        if (pO && !Core::Hook::CallMethod<bool>(0xA3C6E0, pO))
            Core::Hook::Call<void>(0x987730, 22, &m_pMatrix->pos, this, 0, 0, 0.0f);
    }

    if ((m_nVehFlagsF67 & 8u) != 0 && *reinterpret_cast<const int32*>(self + 0x144) == 2)
        ResetHealthAndSubObjFD0();

    reinterpret_cast<void(__thiscall*)(CVehicle*)>(m_vftable[107])(this);
}
