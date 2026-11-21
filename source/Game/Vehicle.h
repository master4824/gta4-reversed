#pragma once
#include "Game/Physical.h"

class CPed;

class CVehicle : public CPhysical
{
public:
    uint8  pad210[0x40];                  // +0x210..+0x24F
    uint8  m_nVehUnk250;                  // +0x250
    uint8  pad251[0xF];                   // +0x251..+0x25F
    uint8  m_vehSubObj260[0xB50];         // +0x260..+0xDAF
    uint8  m_vehAudioEntity[0x58];        // +0xDB0..+0xE07
    uint8  m_audPlaceableTracker[0x0C];   // +0xE08..+0xE13
    void*  m_pFragInstE14;                // +0xE14
    uint8  padE18[8];                     // +0xE18..+0xE1F
    uint32 m_nVehBoneMaskE20;             // +0xE20
    uint8  m_vehSubObjE24[0x10];          // +0xE24..+0xE33
    uint16 m_nVehSubObjE34;               // +0xE34
    uint8  padE36[0x52];                  // +0xE36..+0xE87
    uint32 m_nVehPretendOccTimeMs;        // +0xE88
    uint8  padE8C[0x64];                  // +0xE8C..+0xEEF
    uint8  m_bHasPretendOccupants;        // +0xEF0
    uint8  padEF1[3];                     // +0xEF1..+0xEF3
    uint8  m_vehSubObjEF4[0x70];          // +0xEF4..+0xF63
    uint8  m_nVehFlagsF64;                // +0xF64
    uint8  m_nVehFlagsF65;                // +0xF65
    uint8  m_nVehFlagsF66;                // +0xF66
    uint8  m_nVehFlagsF67;                // +0xF67
    uint8  m_nVehFlagsF68;                // +0xF68
    uint8  m_nVehFlagsF69;                // +0xF69
    uint8  m_nVehFlagsF6A;                // +0xF6A
    uint8  m_nVehFlagsF6B;                // +0xF6B
    uint8  m_nVehFlagsF6C;                // +0xF6C
    uint8  m_nVehFlagsF6D;                // +0xF6D
    uint8  m_nVehFlagsF6E;                // +0xF6E
    uint8  m_nVehFlagsF6F;                // +0xF6F
    uint8  m_nVehFlagsF70;                // +0xF70
    uint8  m_nVehFlagsF71;                // +0xF71
    uint8  padF72[2];                     // +0xF72..+0xF73
    uint32 m_nVehUnkF74;                  // +0xF74
    int32  m_nVehUnkF78;                  // +0xF78
    uint32 m_nVehUnkF7C;                  // +0xF7C
    uint32 m_nVehUnkF80;                  // +0xF80
    float  m_fVehUnkF84;                  // +0xF84
    uint32 m_nVehUnkF88;                  // +0xF88
    uint8  padF8C[0xC];                   // +0xF8C..+0xF97
    int16  m_nVehUnkF98;                  // +0xF98
    int16  m_nVehUnkF9A;                  // +0xF9A
    int16  m_nVehUnkF9C;                  // +0xF9C
    uint8  padF9E[2];                     // +0xF9E..+0xF9F
    CPed*  m_pDriver;                     // +0xFA0
    CPed*  m_pPassengers[8];              // +0xFA4..+0xFC3
    float  m_fVehUnkFC4;                  // +0xFC4
    uint8  padFC8[4];                     // +0xFC8..+0xFCB
    uint8  m_nVehUnkFCC;                  // +0xFCC
    uint8  padFCD[3];                     // +0xFCD..+0xFCF
    uint32 m_nVehUnkFD0;                  // +0xFD0
    uint32 m_nVehUnkFD4;                  // +0xFD4
    uint32 m_nVehUnkFD8;                  // +0xFD8
    uint32 m_nVehUnkFDC;                  // +0xFDC
    uint32 m_nVehUnkFE0;                  // +0xFE0
    uint8  padFE4[8];                     // +0xFE4..+0xFEB
    uint32 m_nVehUnkFEC;                  // +0xFEC
    uint8  padFF0[4];                     // +0xFF0..+0xFF3
    int32  m_nVehUnkFF4;                  // +0xFF4
    uint32 m_nVehUnkFF8;                  // +0xFF8
    uint8  m_vehSubObjFFC[8 * 0x14];      // +0xFFC..+0x109B
    uint32 m_nVehUnk109C;                 // +0x109C
    float  m_fVehUnk10A0;                 // +0x10A0
    float  m_fVehUnk10A4;                 // +0x10A4
    float  m_fVehUnk10A8;                 // +0x10A8
    uint8  pad10AC[4];                    // +0x10AC..+0x10AF
    float  m_fVehUnk10B0;                 // +0x10B0
    float  m_fVehUnk10B4;                 // +0x10B4
    float  m_fVehUnk10B8;                 // +0x10B8
    uint8  pad10BC[4];                    // +0x10BC..+0x10BF
    uint8  m_nMaxPassengers;              // +0x10C0
    uint8  m_nVehUnk10C1;                 // +0x10C1
    uint8  m_nVehUnk10C2;                 // +0x10C2
    uint8  pad10C3;                       // +0x10C3
    int16  m_nVehUnk10C4;                 // +0x10C4
    uint8  pad10C6[2];                    // +0x10C6..+0x10C7
    float  m_fVehUnk10C8;                 // +0x10C8
    float  m_fVehUnk10CC;                 // +0x10CC
    float  m_fVehUnk10D0;                 // +0x10D0
    float  m_fVehUnk10D4;                 // +0x10D4
    float  m_fVehUnk10D8;                 // +0x10D8
    float  m_fVehUnk10DC;                 // +0x10DC
    uint8  m_vehSubObj10E0[0x28];         // +0x10E0..+0x1107
    uint8  m_nCreatedBy;                  // +0x1108
    uint8  pad1109[3];                    // +0x1109..+0x110B
    uint32 m_nVehUnk110C;                 // +0x110C
    int16  m_nVehUnk1110;                 // +0x1110
    uint8  m_nVehUnk1112;                 // +0x1112
    uint8  pad1113;                       // +0x1113
    int16  m_nVehUnk1114;                 // +0x1114
    uint8  pad1116[2];                    // +0x1116..+0x1117
    float  m_fVehUnk1118;                 // +0x1118
    float  m_fVehFadeScale111C;           // +0x111C
    uint8  m_vehSubObj1120[8];            // +0x1120..+0x1127
    float  m_fVehUnk1128;                 // +0x1128
    uint8  pad112C[0x94];                 // +0x112C..+0x11BF
    uint8  m_nVehUnk11C0;                 // +0x11C0
    uint8  pad11C1[0x1F];                 // +0x11C1..+0x11DF
    uint8  m_nVehBoneResetMaskA[13];      // +0x11E0..+0x11EC
    uint8  pad11ED;                       // +0x11ED
    uint8  m_bVehBoneResetA;              // +0x11EE
    uint8  m_bVehBoneResetB;              // +0x11EF
    uint8  m_nVehBoneResetMaskB[8];       // +0x11F0..+0x11F7
    uint8  pad11F8[0xE8];                 // +0x11F8..+0x12DF
    uint32 m_nVehUnk12E0;                 // +0x12E0
    uint32 m_nVehUnk12E4;                 // +0x12E4
    uint32 m_nVehUnk12E8;                 // +0x12E8
    uint32 m_nVehUnk12EC;                 // +0x12EC
    uint32 m_nVehUnk12F0;                 // +0x12F0
    uint32 m_nVehUnk12F4;                 // +0x12F4
    uint32 m_nVehUnk12F8;                 // +0x12F8
    uint32 m_nVehUnk12FC;                 // +0x12FC
    uint32 m_nVehUnk1300;                 // +0x1300
    float  m_fVehUnk1304;                 // +0x1304
    uint32 m_nVehUnk1308;                 // +0x1308
    int16  m_nVehUnk130C;                 // +0x130C
    int16  m_nVehUnk130E;                 // +0x130E
    uint32 m_nVehUnk1310;                 // +0x1310
    uint32 m_nVehUnk1314;                 // +0x1314
    float  m_fVehUnk1318;                 // +0x1318
    float  m_fVehUnk131C;                 // +0x131C
    int32  m_nVehDoorLockState1320;       // +0x1320
    float  m_fVehUnk1324;                 // +0x1324
    float  m_fVehUnk1328;                 // +0x1328
    float  m_fVehUnk132C;                 // +0x132C
    float  m_fVehUnk1330;                 // +0x1330
    uint8  pad1334;                       // +0x1334
    uint8  m_nVehUnk1335;                 // +0x1335
    uint8  m_nVehUnk1336;                 // +0x1336
    uint8  m_nVehUnk1337;                 // +0x1337
    uint32 m_nVehUnk1338;                 // +0x1338
    uint32 m_nVehUnk133C;                 // +0x133C
    uint8  m_nVehUnk1340;                 // +0x1340
    uint8  m_nVehUnk1341;                 // +0x1341
    uint8  m_nVehUnk1342;                 // +0x1342
    uint8  m_nVehUnk1343;                 // +0x1343
    uint8  m_nVehUnk1344;                 // +0x1344
    uint8  pad1345[3];                    // +0x1345..+0x1347
    float  m_fVehUnk1348;                 // +0x1348
    uint8  m_nVehUnk134C;                 // +0x134C
    uint8  pad134D[3];                    // +0x134D..+0x134F

    static void InjectHooks();

    CVehicle* __thiscall Ctor(uint8 createdBy);
    CPed*     __thiscall GetPassenger(int32 seat);

    static int32 __stdcall SeatToPassengerIndex(int32 seat);
    static int32 __stdcall PassengerIndexToSeat(int32 index);

    int32 GetVehicleSubType() const { return *reinterpret_cast<const int32*>(reinterpret_cast<const uint8*>(this) + 0x1350); }
    int32 GetVehicleType() const    { return *reinterpret_cast<const int32*>(reinterpret_cast<const uint8*>(this) + 0x1354); }
    float GetHeliRotorSpeed() const { return *reinterpret_cast<const float*>(reinterpret_cast<const uint8*>(this) + 0x1F24); }
    int8  GetBoatUnk1F40(int32 idx) const { return *reinterpret_cast<const int8*>(reinterpret_cast<const uint8*>(this) + 0x1F40 + idx); }

    void        __thiscall ResetSubObjAndAudio();
    bool        __thiscall IsFlagF6D10Set();
    bool        __thiscall TrySetFlagF6D10(bool updateTimer);
    bool        __thiscall GetTimer12F4Progress(float* outProgress);
    bool        __thiscall IsUsingPretendOccupants();
    static int32 __stdcall SeatToDoorIndex(int32 seat);
    int32       __thiscall GetPedSeat(CPed* pPed);
    int32       __thiscall GetSeatForTargetId(int32 targetId);
    int32       __thiscall GetAdjustedSeatIndex(int32 a1);
    const char* __thiscall GetSeatNameById(int32 id);
    void        __thiscall UpdateVehUnkF84(float t);
    bool        __thiscall IsPlayerInVehicle();

    bool        __thiscall HasDummyOccupants();
    bool        __thiscall HasAnyOccupants();
    bool        __thiscall HasAnyUninjuredOccupants();
    void*       __thiscall FindSubObjFD0(int32 key);
    int32       __thiscall CountActiveSubObjFD0();
    void*       __thiscall FindSubObjFD8(int32 key);
    static void __cdecl    GetModelBounds(int32 modelIdx, float* outA, float* outB, uint8 a4);
    void        __thiscall UpdateTimer130E();
    static bool __stdcall  IsObjectFlagSet(const uint8* pObj);
    void        __thiscall RemovePassenger(CPed* pPed);
    void*       __thiscall GetModelSeatEntry(int32 idx);
    int32       __thiscall GetModelSeatCount();

    bool        __thiscall HasAnyActiveLink();
    int32       __thiscall FindLinkIndexByKey(int32 key);
    void*       __thiscall GetLink(int32 idx);
    void*       __thiscall AllocateLink(int32 val);
    static void __cdecl    InitVehicleConfig();
    int32       __thiscall GetPassengerCount();
    void        __thiscall SetOccupantsPropFlag100(bool setFlag);
    void        __thiscall AddVehUnk10A0(const float* pVec3);
    void        __thiscall AddVehUnk10B0(const float* pVec3);
    void        __thiscall AddCrossVehUnk10B0(const float* pA, const float* pB);
    bool        __thiscall IsAudioSubTypeF();
    void        __thiscall ProcessMovingAudio1348();

    void        __thiscall ProcessOccupantDamageF67();
    int32       __thiscall GetDrawHandlerUnkD8();
    void        __thiscall SetDrawHandlerUnkD8(int32 val);
    void*       __thiscall GetBoneLocalMatrix(int32 boneIdx);

    static void __cdecl    InitSystem();
    int32       __thiscall ProcessControlStub(int32 a2, int32 a3, int32 a4, int32 a5, int32 a6);
    int32       __thiscall GetDefaultMaxPassengers();
    int32       __thiscall PreRender();
    void*       __thiscall CreateModelInstance(int32 a2, int32 a3, int32 a4, int32 a5);
    bool        __thiscall GetBoneWorldPos(int32 boneIdx, float* outVec4);
    void        __thiscall GetBoneMatrix(int32 boneId, float* outMat4x4);
    void        __thiscall UpdateDamageBoneTable();
    bool        __thiscall CanBeConvertedToDummy(bool a2, bool a3, int32 a4, int32 a5, bool a6, bool a7, bool a8);
    bool        __thiscall CanUsePretendOccupants();
    bool        __thiscall TrySpawnInWorld(bool a2, bool a3, int32 a4);

    bool        __thiscall IsEnterable();
    bool        __thiscall TryEnablePretendOccupants();
    void        __thiscall ClearOccupantsAndSetFlag400();
    bool        __thiscall TryDisablePretendOccupants();
    void        __thiscall CollectEntityAndOccupants(void** outArr, int32* pCount, int32 maxCount, void* pExtra);
    bool        __thiscall CheckVehicleUprightAndState();
    void        __thiscall UpdateWheelSteerOrLean(uint8* pArg, bool bFlag);
    bool        __thiscall AddPassenger(CPed* pPed);
    bool        __thiscall AddPassengerAtSeat(CPed* pPed, uint32 seatIdx);
    void        __thiscall ProcessBurningOccupants(void* a2, int32 a3);
    bool        __thiscall IsUpsideDown();
    bool        __thiscall IsOnSide();

    bool        __thiscall CanPedEnterWithLockState(CPed* pPed);
    bool        __thiscall IsVehicleStationaryAndLevel(bool bSkipVelCheck);
    bool        __thiscall ShouldPedBailFromSpeed(CPed* pPed);
    bool        __thiscall TryStartTimerF98();
    static bool __cdecl    IsSpecialModelIndex(int32 modelIdx);
    void        __thiscall ResetHealthAndSubObjFD0();
    void        __thiscall PlayPedEnterAnim(CPed* pPed, int32 a3);
    bool        __thiscall InitPretendOccupants();
    CVehicle*   __thiscall ScalarDeletingDtor(uint8 flags);
    void        __thiscall InitFragAndPhysics();
    void        __thiscall CleanupPhysicsAndAudio();
    void        __thiscall SetHeading(void* a2, float angle, int32 a4);

    static bool __cdecl    ArePedsAwareOrHostile(CPed* pPedA, CPed* pPedB);
    void        __thiscall ResetDrawHandler();

    bool        __thiscall SetDriver(CPed* pPed, uint32 arg4);
    void        __thiscall UpdateUpsideDownAlarmOrFireFlag();
    bool        __thiscall IsOnFireFlagActive();
    static double __stdcall GetExcessSpeedOrInvalid(float speed, int32 unused);
    CPed*       __thiscall GetRandomPassenger();
    bool        __thiscall HasAnyActiveOccupant219();
    bool        __thiscall HasAliveOccupants();
    bool        __thiscall IsPedOccupant(const CPed* pPed);
    bool        __thiscall IsInActiveRange(void* unused);
    bool        __thiscall CanBeEnteredByPed();
    void        __thiscall DecrementUnk133C();
    bool        __thiscall ShouldSkipLodOrUpdate();

    int32       __thiscall GetVehicleClassFromFlags();
    void        __thiscall RemoveDriver(bool bSetFlag);
    CPed*       __thiscall TryAddRandomDriver(int32 arg);
    CPed*       __thiscall TryAddRandomPassenger(int32 seatIdx);
    bool        __thiscall IsPedPassenger(const CPed* pPed);
    bool        __thiscall IsPedDriver(const CPed* pPed);
    bool        __thiscall IsDoorLockStateUnlocked();
    void        __thiscall ProcessAlarm();
    bool        __thiscall IsPointInBoundingBox(float x, float y, float z, float margin);
    static int32 __cdecl   GetVehicleAnimGroupIndex(int32 animGroupId);
    static bool __cdecl    IsSpecialVehicleModel(int32 modelIdx);
    bool        __thiscall IsConvertibleRoofClosedOrValid();

    void        __thiscall ResetBoneChild(int32 boneSlot);
    void        __thiscall SetBoneHidden(int32 boneSlot, bool bHide);
    bool        __thiscall IsBoneVisible(int32 boneSlot);
    void        __thiscall SetCreatedBy(uint8 createdBy);
    void        __thiscall ClearUnkFlagsF64_1344();
    double      __thiscall GetBurnoutOrEngineHealthFactor();
    bool        __thiscall HasAnyWheelUnk160Not1000();
    bool        __thiscall HasAnyWheelUnk15CNot1000();
    int32       __thiscall GetModelInfoEntryIndex(void* pEntry);
    void        __thiscall ResolveContactPenetration(void* pContacts);
    void        __thiscall ProcessFlyingPhysics(int32 nMode, float fTimeStep, float a4, float a5, float a6, float a7, float a8);

    void*       __thiscall FindOrAllocateLink(int32 val, bool bCreate);
    void        __thiscall ProcessLinks();
    void*       __thiscall GetLinkForSeat(int32 seatSlot, int32 seatIdx);
    void*       __thiscall GetLinkForSeatSlot(int32 seatSlot);
    void        __thiscall GetUnkSeatOffset(float* pOutVec4, int32 doorOrSeat, int32 a4);
    bool        __thiscall SaveStateSnapshot();
    bool        __thiscall LoadStateSnapshot();
    bool        __thiscall IsBoneNonZero(int32 boneSlot);
    bool        __thiscall ResetBoneChildIfNonZero(int32 boneSlot);
    static void* __cdecl   InitVehiclePool();
    void        __thiscall ApplyHiddenBoneMask();
    void        __thiscall UpdateSubObj80Position();

    void        __thiscall ProcessDriverDropWeapon();
    void        __thiscall ProcessCollisions(void** pContacts, void* pEntity, int32 startIdx, int32 count);
    void        __thiscall InitBrokenBoneMask(int16 flags);
    void        __thiscall UpdateDoorOpenRatio(void* pUnused, int32 doorId, uint8* pBone, bool bFlag);
    void        __thiscall UpdateDoorCloseRatio(void* pUnused, int32 doorId, uint8* pBone, bool bFlag);
    void        __thiscall UpdateUnk10D8_10C8_10CC(float* pVal1, int32 a3, float* pVal2, float* pVal3, bool bActive, bool bFlag);
    void        __thiscall SetEngineOrBurnoutState(bool bEnable);
    void        __thiscall ProcessEngineOrBurnout();
    void        __thiscall ProcessInteriorLightOrUnk1114(float fTimeStep);
    void        __thiscall UpdateDirtLevel();
    void        __thiscall Dtor();
    void        __thiscall SetModelIndex(int32 modelIndex);

    void        __thiscall ProcessLightsAndEffects(float* pMat, uint32 flags);
    void        __thiscall ApplyModelDoorRotation(int32 doorSlot, float* pMat3x4, uint8* pOutFlag);
    bool        __thiscall IsClearAbove(const float* pPos);
    void        __thiscall UpdateInteriorModelStream();
    bool        __thiscall GetBoneEntryUnkC299A0(int32 boneSlot, float* pOutVec3);
    bool        __thiscall GetBoneEntryUnk9C8900(int32 boneSlot, void* pOut);
    void        __thiscall RegisterInUnk18A82AC(int32 unused);
    double      __thiscall ComputeSeatEntryTransform(CMatrix* outMat, CEntity* pPed, int32 doorIdx, int32 bCheckBlocked, float fProgress, float fBlend, float fOffsetScale, const CVector* pLocalOff, float fExtraZ, bool bForceSide, bool bFlipSide, bool bDoProbe);
    void        __thiscall DetachFragPart(int32 boneSlot, float fImpulse);
    void        __thiscall UpdateNetworkOrPhysicsActive(bool bCheckSub);
    void        __thiscall ProcessControl();

private:
    void        ApplyPassengerImpulse(CPed* pPed);
};

VALIDATE_SIZE(CVehicle, 0x1350);
VALIDATE_OFFSET(CVehicle, m_vehSubObj260, 0x260);
VALIDATE_OFFSET(CVehicle, m_vehAudioEntity, 0xDB0);
VALIDATE_OFFSET(CVehicle, m_audPlaceableTracker, 0xE08);
VALIDATE_OFFSET(CVehicle, m_pDriver, 0xFA0);
VALIDATE_OFFSET(CVehicle, m_pPassengers, 0xFA4);
VALIDATE_OFFSET(CVehicle, m_nMaxPassengers, 0x10C0);
VALIDATE_OFFSET(CVehicle, m_nCreatedBy, 0x1108);
VALIDATE_OFFSET(CVehicle, m_nVehUnk134C, 0x134C);
