#pragma once
#include "PCH.h"
#include "CommonFollowerUtil.h"

namespace Papyrus {
    static inline const char CLASS_NAME[] = "EstrusForSkyrimNG";
    //static inline const uint8_t DangerousWaterFlagMask = 0x01;
    static inline bool DebugOn = true;

    class EstrusForSkyrimNG
    {
    public:
        //static inline RE::TESObjectREFR* GetSelectedReference(RE::RE::StaticFunctionTag*);
        static inline void Debug1(RE::StaticFunctionTag*) { logs::info("Action from papyrus"); }

        static inline RE::Actor* GetFollower(RE::StaticFunctionTag*, bool isFemale, uint32_t number) {
            if (DebugOn) {
                logs::info("call GetFollower:");
            }
            auto ref = isFemale ? FollowerCollection::getFollowerObjectFemale(number)
                                : FollowerCollection::getFollowerObjectMale(number);
            return ref ? ref->As<RE::Actor>() : nullptr;
        }

        static inline RE::Actor* GetRandomFollower(RE::StaticFunctionTag*, bool isFemale) {
            if (DebugOn) {
                logs::info("call GetRandomFollower:");
            }
            auto ref = isFemale ? FollowerCollection::getRandomFollowerObjectFemale()
                                : FollowerCollection::getRandomFollowerObjectMale();
            return ref ? ref->As<RE::Actor>() : nullptr;
        }

        static inline uint32_t GetFollowersCount(RE::StaticFunctionTag*, bool isFemale) {
            if (DebugOn) {
                logs::info("call GetFollowersCount:");
            }
            if (isFemale) {
                return FollowerCollection::getFollowersCountFemale();
            } else {
                return FollowerCollection::getFollowersCountMale();
            }
        }

        static inline bool IsDangerousWater(RE::StaticFunctionTag*, RE::TESForm* waterForm) {
            if (DebugOn) {
                logs::info("call IsDangerousWater:");
            }
            if (!waterForm) {
                logs::info("waterForm is null");
                return false;
            }
            auto water = waterForm->As<RE::TESWaterForm>();
            return water ? water->GetDangerous() : false;
        }

        static inline void SetDangerousWater(RE::StaticFunctionTag*, RE::TESForm* waterForm, bool dangerous)
        {
            if (DebugOn) {
                logs::info("call SetDangerousWater:");
            }
            if (!waterForm) {
                logs::info("waterForm is null");
                return;
            }
            auto water = waterForm->As<RE::TESWaterForm>();
            if (!water) {
                logs::info("form:{:08X} is not Water.", waterForm->formID);
                return;
            }

            if (dangerous) {
                water->flags.set(RE::TESWaterForm::Flag::kCauseDamage);
            } else {
                water->flags.reset(RE::TESWaterForm::Flag::kCauseDamage);
            }
        }

        //Idle function getCurrentIdle(RE::Actor akRE::Actor) global native
        static inline RE::TESIdleForm* GetCurrentIdle(RE::StaticFunctionTag*, RE::Actor* thisActor)
        {
            if (DebugOn) {
                logs::info("call GetCurrentIdle:");
            }
            if (!thisActor) {
                logs::info("RE::Actor is null");
                return NULL;
            }
            auto ai = thisActor->GetActorRuntimeData().currentProcess;
            if (!ai || !ai->high) {
                logs::info("RE::Actor {:08X} has NO ai", thisActor->formID);
                return nullptr;
            }
            auto currentProcessIdle = ai->high->currentProcessIdle;

            if (!currentProcessIdle) {
                logs::info("RE::Actor {:08X} 's current is null", thisActor->formID);
                return NULL;
            }

            return currentProcessIdle;
        }

        // Delete: Use Papyrus Actor.GetFurnitureReference as a replacement for the following
        // RE::TESObjectREFR* GetFurnitureReference(RE::StaticFunctionTag*, RE::Actor* thisActor);

        // Delete: Use Papyrus ArmorAddon.GetModelNthTextureSet as a replacement for the following
        // TextureSet function GetDefaultSkinTextureSet(ArmorAddon thisArmorAddon, Bool bFemale) global native

        static inline RE::TESNPC::HeadPartType GetHeadPartType(uint32_t type)
        {
            if (type == 0)
                return RE::TESNPC::HeadPartType::kMisc;
            else if (type == 1)
                return RE::TESNPC::HeadPartType::kFace;
            else if (type == 2)
                return RE::TESNPC::HeadPartType::kEyes;
            else if (type == 3)
                return RE::TESNPC::HeadPartType::kHair;
            else if (type == 4)
                return RE::TESNPC::HeadPartType::kFacialHair;
            else if (type == 5)
                return RE::TESNPC::HeadPartType::kScar;
            else if (type == 6)
                return RE::TESNPC::HeadPartType::kEyebrows;
            else
                return RE::TESNPC::HeadPartType::kTotal;
        }

        static inline RE::BGSHeadPart* GetHeadPart(RE::Actor* thisActor, uint32_t iType)
        {
            if (!thisActor) return nullptr;
            auto npc = thisActor->GetActorBase();
            if (!npc) return NULL;
            return npc->GetHeadPartByType(GetHeadPartType(iType));
        }

        // String function GetTriFilePath(RE::Actor akRE::Actor, Int type) global native
        static inline RE::BSFixedString GetTriFilePath(RE::StaticFunctionTag*, RE::Actor* thisActor, uint32_t iType)
        {
            if (DebugOn) {
                logs::info("call GetTriFilePath:");
            }
            if (!thisActor) {
                logs::info("RE::Actor is null");
                return "";
            }
            auto hp = GetHeadPart(thisActor, iType);
            return (hp) ? hp->morphs[0].GetModel() : "";
        }

        //void SwitchTriFiles(RE::StaticFunctionTag*, RE::Actor* thisActor, BSFixedString pathHead, BSFixedString pathEyes,BSFixedString pathBrows)
        static inline void SetTriFilePath(RE::StaticFunctionTag*, RE::Actor* thisActor, uint32_t iType, RE::BSFixedString triPath)
        {
            if (DebugOn) {
                logs::info("call SetTriFilePath:");
            }
            if (!thisActor) {
                logs::info("RE::Actor is null");
                return;
            }
            auto hp = GetHeadPart(thisActor, iType);
            if (!hp) {
                logs::info("RE::Actor {:08X} has no head part of type {}", thisActor->formID, iType);
                return;
            }
            hp->morphs[0].SetModel(triPath.c_str());
        }

        // String function GetAnimObjectPath(AnimObject) global native
        static inline RE::BSFixedString GetAnimObjectPath(RE::StaticFunctionTag*, RE::TESForm* thisForm)
        {
            if (DebugOn) {
                logs::info("call GetAnimObjectPath:");
            }
            if (!thisForm) {
                logs::info("RE::TESForm is null");
                return "";
            }
            auto anim = thisForm->As<RE::TESObjectANIO>();
            return (anim) ? anim->GetModel() : "";
        }

        // function SetAnimObjectPath(AnimObject anim, String newPath) global native
        static inline void SetAnimObjectPath(RE::StaticFunctionTag*, RE::TESForm* thisForm, RE::BSFixedString newPath)
        {
            if (!thisForm) {
                logs::info("RE::TESForm is null");
                return;
            }
            if (DebugOn) {
                logs::info("call SetAnimObjectPath: thisForm:{:08X}, newPath:{}", thisForm->formID, newPath.c_str());
            }
            auto anim = thisForm->As<RE::TESObjectANIO>();
            if (!anim) return;
            anim->SetModel(newPath.c_str());
        }

        static inline bool Register(RE::BSScript::IVirtualMachine* a_vm)
        {
            a_vm->RegisterFunction("Debug1", CLASS_NAME, Debug1);
            a_vm->RegisterFunction("GetFollower", CLASS_NAME, GetFollower);
            a_vm->RegisterFunction("GetRandomFollower", CLASS_NAME, GetRandomFollower);
            a_vm->RegisterFunction("GetFollowersCount", CLASS_NAME, GetFollowersCount);
            a_vm->RegisterFunction("IsDangerousWater", CLASS_NAME, IsDangerousWater);
            a_vm->RegisterFunction("SetDangerousWater", CLASS_NAME, SetDangerousWater);
            a_vm->RegisterFunction("GetCurrentIdle", CLASS_NAME, GetCurrentIdle);
            a_vm->RegisterFunction("GetTriFilePath", CLASS_NAME, GetTriFilePath);
            a_vm->RegisterFunction("SetTriFilePath", CLASS_NAME, SetTriFilePath);
            a_vm->RegisterFunction("GetAnimObjectPath", CLASS_NAME, GetAnimObjectPath);
            a_vm->RegisterFunction("SetAnimObjectPath", CLASS_NAME, SetAnimObjectPath);
            logs::info("Registered funcs for class {}", CLASS_NAME);
            return true;
        }
    };


    inline bool Register(RE::BSScript::IVirtualMachine* a_vm)
    {
        EstrusForSkyrimNG::Register(a_vm);
        return true;
    }
}