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
            if (isFemale) {
                return (RE::Actor*)FollowerCollection::getFollowerObjectFemale(number);
            } else {
                return (RE::Actor*)FollowerCollection::getFollowerObjectMale(number);
            }
        }

        static inline RE::Actor* GetRandomFollower(RE::StaticFunctionTag*, bool isFemale) {
            if (DebugOn) {
                logs::info("call GetRandomFollower:");
            }
            if (isFemale) {
                return (RE::Actor*)FollowerCollection::getRandomFollowerObjectFemale();
            } else {
                return (RE::Actor*)FollowerCollection::getRandomFollowerObjectMale();
            }
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
            if (waterForm->GetFormType() != RE::FormType::Water) {
                return false;
            }

            return dynamic_cast<RE::TESWaterForm*>(waterForm)->GetDangerous();
        }

        static inline void SetDangerousWater(RE::StaticFunctionTag*, RE::TESForm* waterForm, bool dangerous)
        {
            if (DebugOn) {
                logs::info("call SetDangerousWater:");
            }
            logs::info("waterForm {:08X} has type {}", waterForm->formID, waterForm->GetFormType());
            if (!waterForm) {
                logs::info("waterForm is null");
                return;
            }
            if (waterForm->GetFormType() != RE::FormType::Water) {
                logs::info("form:{:08X} is not Water.", waterForm->formID);
                return;
            }

            if (dangerous) {
                //logs::info("this route is true");
                dynamic_cast<RE::TESWaterForm*>(waterForm)->flags.set(RE::TESWaterForm::Flag::kCauseDamage);
            } else {
                //logs::info("this route is false");
                dynamic_cast<RE::TESWaterForm*>(waterForm)->flags.set(RE::TESWaterForm::Flag::kNone);
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
            if (!ai) {
                logs::info("RE::Actor {:08X} has NO ai", thisActor->formID);
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
            }
            auto hp = GetHeadPart(thisActor, iType);
            return (hp) ? hp->morphs->GetModel() : NULL;
        }

        //void SwitchTriFiles(RE::StaticFunctionTag*, RE::Actor* thisActor, BSFixedString pathHead, BSFixedString pathEyes,BSFixedString pathBrows) 
        static inline void SetTriFilePath(RE::StaticFunctionTag*, RE::Actor* thisActor, uint32_t iType, RE::BSFixedString triPath)
        {
            if (DebugOn) {
                logs::info("call SetTriFilePath:");
            }
            if (!thisActor) {
                logs::info("RE::Actor is null");
            }
            auto hp = GetHeadPart(thisActor, iType);
            hp->morphs->SetModel(triPath.c_str());
        }

        // String function GetAnimObjectPath(AnimObject) global native
        static inline RE::BSFixedString GetAnimObjectPath(RE::StaticFunctionTag*, RE::TESForm* thisForm) 
        {
            if (DebugOn) {
                logs::info("call GetAnimObjectPath:");
            }
            if (!thisForm) {
                logs::info("RE::TESForm is null");
            }
            auto anim = RE::TESForm::LookupByID<RE::TESObjectANIO>(thisForm->formID);
            return (anim) ? anim->alternateTextures->name3D : NULL;
        }

        // function SetAnimObjectPath(AnimObject anim, String newPath) global native
        static inline void SetAnimObjectPath(RE::StaticFunctionTag*, RE::TESForm* thisForm, RE::BSFixedString newPath) 
        {
            if (DebugOn) {
                logs::info("call SetAnimObjectPath: thisForm:{:08X}, newPath:{}", thisForm->formID, newPath);
            }
            if (!thisForm) {
                logs::info("RE::TESForm is null");
            }
            auto anim = RE::TESForm::LookupByID<RE::TESObjectANIO>(thisForm->formID);
            if (!anim) return;
            anim->model = newPath;
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