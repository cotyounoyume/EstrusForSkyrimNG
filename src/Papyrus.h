#pragma once
#include "PCH.h"
#include "CommonFollowerUtil.h"

namespace Papyrus {
    static inline const char CLASS_NAME[] = "EstrusForSkyrimNG";
    //static inline const uint8_t DangerousWaterFlagMask = 0x01;

    class EstrusForSkyrimNG 
    {
    public:
        //static inline RE::TESObjectREFR* GetSelectedReference(RE::RE::StaticFunctionTag*);
        static inline void Debug1(RE::StaticFunctionTag*) { logs::info("Action from papyrus"); }

        static inline RE::Actor* getFollower(RE::StaticFunctionTag*, bool isFemale, uint32_t number) {
            if (isFemale) {
                return (RE::Actor*)FollowerCollection::getFollowerObjectFemale(number);
            } else {
                return (RE::Actor*)FollowerCollection::getFollowerObjectMale(number);
            }
        }

        static inline RE::Actor* getRandomFollower(RE::StaticFunctionTag*, bool isFemale) {
            if (isFemale) {
                return (RE::Actor*)FollowerCollection::getRandomFollowerObjectFemale();
            } else {
                return (RE::Actor*)FollowerCollection::getRandomFollowerObjectMale();
            }
        }

        static inline uint32_t getFollowersCount(RE::StaticFunctionTag*, bool isFemale) {
            if (isFemale) {
                return FollowerCollection::getFollowersCountFemale();
            } else {
                return FollowerCollection::getFollowersCountMale();
            }
        }

        static inline bool IsDangerousWater(RE::StaticFunctionTag*, RE::TESForm* waterForm) {
            if (waterForm->GetFormType() != RE::FormType::Water) {
                return false;
            }

            return dynamic_cast<RE::TESWaterForm*>(waterForm)->GetDangerous();
        }

        static inline void SetDangerousWater(RE::StaticFunctionTag*, RE::TESForm* waterForm, bool dangerous)
        {
            logs::info("waterForm {:08X} has type {}", waterForm->formID, waterForm->GetFormType());
            if (waterForm->GetFormType() != RE::FormType::Water) {
                logs::info("this route is failure");
                return;
            }

            if (dangerous) {
                logs::info("this route is true");
                dynamic_cast<RE::TESWaterForm*>(waterForm)->flags.set(RE::TESWaterForm::Flag::kCauseDamage);
            } else {
                logs::info("this route is false");
                dynamic_cast<RE::TESWaterForm*>(waterForm)->flags.set(RE::TESWaterForm::Flag::kNone);
            }
        }

        //Idle function getCurrentIdle(RE::Actor akRE::Actor) global native
        static inline RE::TESIdleForm* GetCurrentIdle(RE::StaticFunctionTag*, RE::Actor* thisActor) 
        {
            if (!thisActor) {
                logs::info("RE::Actor %08X is null", thisActor->formID);
                return NULL;
            }
            auto ai = thisActor->GetActorRuntimeData().currentProcess;
            auto currentProcessIdle = ai->high->currentProcessIdle;
        
            if (!currentProcessIdle) {
                logs::info("RE::Actor %08X 's current is null", thisActor->formID);
                return NULL;
            }
        
            logs::info("RE::Actor %08X 's currentIdle formID is %08X", thisActor->formID, currentProcessIdle->formID);
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
            return npc->GetHeadPartByType(GetHeadPartType(iType));
        }

        // String function GetTriFilePath(RE::Actor akRE::Actor, Int type) global native
        static inline RE::BSFixedString GetTriFilePath(RE::StaticFunctionTag*, RE::Actor* thisActor, uint32_t iType) {
            auto hp = GetHeadPart(thisActor, iType);
            return (hp) ? hp->morphs->GetModel() : NULL;
        }

        //                 
        // function SwitchTriFiles(RE::Actor akRE::Actor, String headPath, String eyesPath, String browsPath) global native 

        //void SwitchTriFiles(RE::StaticFunctionTag*, RE::Actor* thisActor, BSFixedString pathHead, BSFixedString pathEyes,BSFixedString pathBrows) 
        static inline void SwitchTriFiles(RE::StaticFunctionTag*, RE::Actor* thisActor, uint32_t iType,
                                          RE::BSFixedString triPath)
        {
            auto hp = GetHeadPart(thisActor, iType);
            hp->morphs->SetModel(triPath.c_str());
        }

        // String function GetAnimObjectPath(AnimObject) global native
        static inline RE::BSFixedString GetAnimObjectPath(RE::StaticFunctionTag*, RE::TESForm* thisForm) 
        {
            auto anim = RE::TESForm::LookupByID<RE::TESObjectANIO>(thisForm->formID);
            return (anim) ? anim->alternateTextures->name3D : NULL;
        }


        // function SetAnimObjectPath(AnimObject anim, String newPath) global native
        static inline void SetAnimObjectPath(RE::StaticFunctionTag*, RE::TESForm* thisForm, RE::BSFixedString newPath) 
        {
            auto anim = RE::TESForm::LookupByID<RE::TESObjectANIO>(thisForm->formID);
            anim->alternateTextures->name3D = newPath;
        }








        static inline bool Register(RE::BSScript::IVirtualMachine* a_vm) 
        {
            a_vm->RegisterFunction("Debug1", CLASS_NAME, Debug1);
            a_vm->RegisterFunction("getRandomFollower", CLASS_NAME, getRandomFollower);
            a_vm->RegisterFunction("getFollowersCount", CLASS_NAME, getFollowersCount);
            a_vm->RegisterFunction("IsDangerousWater", CLASS_NAME, IsDangerousWater);
            a_vm->RegisterFunction("SetDangerousWater", CLASS_NAME, SetDangerousWater);
            a_vm->RegisterFunction("GetCurrentIdle", CLASS_NAME, GetCurrentIdle);
            a_vm->RegisterFunction("GetTriFilePath", CLASS_NAME, GetTriFilePath);
            a_vm->RegisterFunction("SwitchTriFiles", CLASS_NAME, SwitchTriFiles);
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