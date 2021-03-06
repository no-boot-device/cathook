/*
 * Misc.cpp
 *
 *  Created on: Nov 5, 2016
 *      Author: nullifiedcat
 */

#include "Misc.h"

#include <unistd.h>

#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <pwd.h>
#include <fcntl.h>

#include "../beforecheaders.h"
#include <fstream>
#include "../aftercheaders.h"

#include <link.h>
#include "../sharedobj.h"

#include "../hack.h"
#include "../common.h"
#include "../sdk.h"
#include "../hooks/hookedmethods.h"
#include "../netmessage.h"
#include "../copypasted/CSignature.h"

namespace hacks { namespace shared { namespace misc {

void CreateMove() {
	static bool flswitch = false;

	if (TF2C && tauntslide)
		RemoveCondition(LOCAL_E, TFCond_Taunting);
	bool crit = IsAttackACrit(g_pUserCmd);
	if (crit_suppress && !crit_hack && WeaponCanCrit()) {
		if (crit && !IsPlayerCritBoosted(LOCAL_E)) {
			g_pUserCmd->buttons &= ~IN_ATTACK;
		}
	} else if (((GetWeaponMode(LOCAL_E) == weapon_melee && crit_melee) || crit_hack) && RandomCrits() && WeaponCanCrit()) {
		if (!crit) g_pUserCmd->buttons &= ~IN_ATTACK;
	}

	if (flashlight_spam) {
		if (flswitch && !g_pUserCmd->impulse) g_pUserCmd->impulse = 100;
		flswitch = !flswitch;
	}
	if (anti_afk) {
		g_pUserCmd->sidemove = RandFloatRange(-450.0, 450.0);
		g_pUserCmd->forwardmove  = RandFloatRange(-450.0, 450.0);
		g_pUserCmd->buttons = rand();
	}
}

void Draw() {
	if (crit_info && CE_GOOD(LOCAL_W)) {
		if (crit_hack) {
			AddCenterString("FORCED CRITS!", colors::red);
		}
		if (TF2) {
			if (!vfunc<bool(*)(IClientEntity*)>(RAW_ENT(LOCAL_W), 465, 0)(RAW_ENT(LOCAL_W)))
				AddCenterString("Random crits are disabled", colors::yellow);
			else {
				if (!vfunc<bool(*)(IClientEntity*)>(RAW_ENT(LOCAL_W), 465 + 21, 0)(RAW_ENT(LOCAL_W)))
					AddCenterString("Weapon can't randomly crit", colors::yellow);
				else
					AddCenterString("Weapon can randomly crit");
			}
			AddCenterString(format("Bucket: %.2f", *(float*)((uintptr_t)RAW_ENT(LOCAL_W) + 2612u)));
		}
	}


	if (!debug_info) return;
		if (CE_GOOD(g_pLocalPlayer->weapon())) {
			/*AddSideString(colors::white, "Weapon: %s [%i]", RAW_ENT(g_pLocalPlayer->weapon())->GetClientClass()->GetName(), g_pLocalPlayer->weapon()->m_iClassID);
			//AddSideString(colors::white, "flNextPrimaryAttack: %f", CE_FLOAT(g_pLocalPlayer->weapon(), netvar.flNextPrimaryAttack));
			//AddSideString(colors::white, "nTickBase: %f", (float)(CE_INT(g_pLocalPlayer->entity, netvar.nTickBase)) * gvars->interval_per_tick);
			AddSideString(colors::white, "CanShoot: %i", CanShoot());
			//AddSideString(colors::white, "Damage: %f", CE_FLOAT(g_pLocalPlayer->weapon(), netvar.flChargedDamage));
			if (TF2) AddSideString(colors::white, "DefIndex: %i", CE_INT(g_pLocalPlayer->weapon(), netvar.iItemDefinitionIndex));
			//AddSideString(colors::white, "GlobalVars: 0x%08x", gvars);
			//AddSideString(colors::white, "realtime: %f", gvars->realtime);
			//AddSideString(colors::white, "interval_per_tick: %f", gvars->interval_per_tick);
			//if (TF2) AddSideString(colors::white, "ambassador_can_headshot: %i", (gvars->curtime - CE_FLOAT(g_pLocalPlayer->weapon(), netvar.flLastFireTime)) > 0.95);
			AddSideString(colors::white, "WeaponMode: %i", GetWeaponMode(g_pLocalPlayer->entity));
			AddSideString(colors::white, "ToGround: %f", DistanceToGround(g_pLocalPlayer->v_Origin));
			AddSideString(colors::white, "ServerTime: %f", CE_FLOAT(g_pLocalPlayer->entity, netvar.nTickBase) * g_GlobalVars->interval_per_tick);
			AddSideString(colors::white, "CurTime: %f", g_GlobalVars->curtime);
			AddSideString(colors::white, "FrameCount: %i", g_GlobalVars->framecount);
			float speed, gravity;
			GetProjectileData(g_pLocalPlayer->weapon(), speed, gravity);
			AddSideString(colors::white, "ALT: %i", g_pLocalPlayer->bAttackLastTick);
			AddSideString(colors::white, "Speed: %f", speed);
			AddSideString(colors::white, "Gravity: %f", gravity);
			AddSideString(colors::white, "CIAC: %i", *(bool*)(RAW_ENT(LOCAL_W) + 2380));
			if (TF2) AddSideString(colors::white, "Melee: %i", vfunc<bool(*)(IClientEntity*)>(RAW_ENT(LOCAL_W), 1860 / 4, 0)(RAW_ENT(LOCAL_W)));
			if (TF2) AddSideString(colors::white, "Bucket: %.2f", *(float*)((uintptr_t)RAW_ENT(LOCAL_W) + 2612u));
			//if (TF2C) AddSideString(colors::white, "Seed: %i", *(int*)(sharedobj::client->lmap->l_addr + 0x00D53F68ul));
			//AddSideString(colors::white, "IsZoomed: %i", g_pLocalPlayer->bZoomed);
			//AddSideString(colors::white, "CanHeadshot: %i", CanHeadshot());
			//AddSideString(colors::white, "IsThirdPerson: %i", iinput->CAM_IsThirdPerson());
			//if (TF2C) AddSideString(colors::white, "Crits: %i", s_bCrits);
			//if (TF2C) AddSideString(colors::white, "CritMult: %i", RemapValClampedNC( CE_INT(LOCAL_E, netvar.iCritMult), 0, 255, 1.0, 6 ));
			for (int i = 0; i < HIGHEST_ENTITY; i++) {
				CachedEntity* e = ENTITY(i);
				if (CE_GOOD(e)) {
					if (e->m_Type == EntityType::ENTITY_PROJECTILE) {
						//logging::Info("Entity %i [%s]: V %.2f (X: %.2f, Y: %.2f, Z: %.2f) ACC %.2f (X: %.2f, Y: %.2f, Z: %.2f)", i, RAW_ENT(e)->GetClientClass()->GetName(), e->m_vecVelocity.Length(), e->m_vecVelocity.x, e->m_vecVelocity.y, e->m_vecVelocity.z, e->m_vecAcceleration.Length(), e->m_vecAcceleration.x, e->m_vecAcceleration.y, e->m_vecAcceleration.z);
						AddSideString(colors::white, "Entity %i [%s]: V %.2f (X: %.2f, Y: %.2f, Z: %.2f) ACC %.2f (X: %.2f, Y: %.2f, Z: %.2f)", i, RAW_ENT(e)->GetClientClass()->GetName(), e->m_vecVelocity.Length(), e->m_vecVelocity.x, e->m_vecVelocity.y, e->m_vecVelocity.z, e->m_vecAcceleration.Length(), e->m_vecAcceleration.x, e->m_vecAcceleration.y, e->m_vecAcceleration.z);
					}
				}
			}//AddSideString(draw::white, draw::black, "???: %f", NET_FLOAT(g_pLocalPlayer->entity, netvar.test));
			//AddSideString(draw::white, draw::black, "VecPunchAngle: %f %f %f", pa.x, pa.y, pa.z);
			//draw::DrawString(10, y, draw::white, draw::black, false, "VecPunchAngleVel: %f %f %f", pav.x, pav.y, pav.z);
			//y += 14;
			//AddCenterString(draw::font_handle, input->GetAnalogValue(AnalogCode_t::MOUSE_X), input->GetAnalogValue(AnalogCode_t::MOUSE_Y), draw::white, L"S\u0FD5");*/
		}
}

void Schema_Reload() {
	static uintptr_t InitSchema_s = gSignatures.GetClientSignature("55 89 E5 57 56 53 83 EC 4C 0F B6 7D 14 C7 04 ? ? ? ? 01 8B 5D 18 8B 75 0C 89 5C 24 04 E8 ? ? ? ? 89 F8 C7 45 C8 00 00 00 00 8D 7D C8 84 C0 8B 45 10 C7 45 CC");
	typedef void(*InitSchema_t)(void*, void*, CUtlBuffer& buffer, bool byte, unsigned version);
	static InitSchema_t InitSchema = (InitSchema_t)InitSchema_s;
	static uintptr_t GetItemSchema_s = gSignatures.GetClientSignature("55 89 E5 83 EC 18 89 5D F8 8B 1D ? ? ? ? 89 7D FC 85 DB 74 12 89 D8 8B 7D FC 8B 5D F8 89 EC 5D C3 8D B6 00 00 00 00 C7 04 24 A8 06 00 00 E8 ? ? ? ? B9 AA 01 00 00 89 C3 31 C0 89 DF");
	typedef void*(*GetItemSchema_t)(void);
	static GetItemSchema_t GetItemSchema = (GetItemSchema_t)GetItemSchema_s;//(*(uintptr_t*)GetItemSchema_s + GetItemSchema_s + 4);

	logging::Info("0x%08x 0x%08x", InitSchema, GetItemSchema);
	void* itemschema = (GetItemSchema() + 4);
	void* data;
	passwd* pwd = getpwuid(getuid());
	char* user = pwd->pw_name;
	char* path = strfmt("/home/%s/.cathook/items_game.txt", user);
	FILE* file = fopen(path, "r");
	delete [] path;
	fseek(file, 0L, SEEK_END);
	char buffer[4 * 1000 * 1000];
	size_t len = ftell(file);
	rewind(file);
	buffer[len + 1] = 0;
	fread(&buffer, sizeof(char), len, file);
	fclose(file);
	CUtlBuffer buf(&buffer, 4 * 1000 * 1000, 9);
	logging::Info("0x%08x 0x%08x", InitSchema, GetItemSchema);
	InitSchema(0, itemschema, buf, false, 0xDEADCA7);
}

CatVar debug_info(CV_SWITCH, "debug_info", "0", "Debug info", "Shows some debug info in-game");
CatVar flashlight_spam(CV_SWITCH, "flashlight", "0", "Flashlight spam", "HL2DM flashlight spam");
CatVar crit_info(CV_SWITCH, "crit_info", "0", "Show crit info"); // TODO separate
CatVar crit_hack(CV_SWITCH, "crit_hack", "0", "Crithack");
CatVar crit_melee(CV_SWITCH, "crit_melee", "0", "Melee crits");
CatVar crit_suppress(CV_SWITCH, "crit_suppress", "0", "Disable random crits", "Can help saving crit bucket for forced crits");
CatVar anti_afk(CV_SWITCH, "anti_afk", "0", "Anti-AFK", "Sends random commands to prevent being kicked from server");
CatVar tauntslide(CV_SWITCH, "tauntslide", "0", "TF2C tauntslide", "Allows moving and shooting while taunting");

CatCommand name("name", "Change name", [](const CCommand& args) {
	if (args.ArgC() < 2) {
		logging::Info("Set a name, silly");
		return;
	}
	if (g_Settings.bInvalid) {
		logging::Info("Only works ingame!");
		return;
	}
	std::string new_name(args.ArgS());
	ReplaceString(new_name, "\\n", "\n");
	NET_SetConVar setname("name", new_name.c_str());
	INetChannel* ch = (INetChannel*)g_IEngine->GetNetChannelInfo();
	if (ch) {
		setname.SetNetChannel(ch);
		setname.SetReliable(false);
		ch->SendNetMsg(setname, false);
	}
});
CatCommand save_settings("save", "Save settings (optional filename)", [](const CCommand& args) {
	std::string filename("lastcfg");
	if (args.ArgC() > 1) {
		filename = std::string(args.Arg(1));
	}
	std::string path = format(g_pszTFPath, "cfg/cat_", filename, ".cfg");
	logging::Info("Saving settings to %s", path.c_str());
	std::ofstream file(path, std::ios::out);
	if (file.bad()) {
		logging::Info("Couldn't open the file!");
		return;
	}
	for (auto i : g_ConVars) {
		if (i) {
			if (strcmp(i->GetString(), i->GetDefault())) {
				file << i->GetName() << " \"" << i->GetString() << "\"\n";
			}
		}
	}
	file.close();
});
CatCommand say_lines("say_lines", "Say with newlines (\\n)", [](const CCommand& args) {
	std::string message(args.ArgS());
	ReplaceString(message, "\\n", "\n");
	std::string cmd = format("say ", message);
	g_IEngine->ServerCmd(cmd.c_str());
});
CatCommand disconnect("disconnect", "Disconnect with custom reason", [](const CCommand& args) {
	INetChannel* ch = (INetChannel*)g_IEngine->GetNetChannelInfo();
	if (!ch) return;
	ch->Shutdown(args.ArgS());
});
CatCommand schema("schema", "Load custom schema", Schema_Reload);
CatCommand disconnect_vac("disconnect_vac", "Disconnect (fake VAC)", []() {
	INetChannel* ch = (INetChannel*)g_IEngine->GetNetChannelInfo();
	if (!ch) return;
	ch->Shutdown("VAC banned from secure server\n");
});
CatCommand set_value("set", "Set value", [](const CCommand& args) {
	if (args.ArgC() < 2) return;
	ConVar* var = g_ICvar->FindVar(args.Arg(1));
	if (!var) return;
	std::string value(args.Arg(2));
	ReplaceString(value, "\\n", "\n");
	var->SetValue(value.c_str());
	logging::Info("Set '%s' to '%s'", args.Arg(1), value.c_str());
});

}}}

/*void DumpRecvTable(CachedEntity* ent, RecvTable* table, int depth, const char* ft, unsigned acc_offset) {
	bool forcetable = ft && strlen(ft);
	if (!forcetable || !strcmp(ft, table->GetName()))
		logging::Info("==== TABLE: %s", table->GetName());
	for (int i = 0; i < table->GetNumProps(); i++) {
		RecvProp* prop = table->GetProp(i);
		if (!prop) continue;
		if (prop->GetDataTable()) {
			DumpRecvTable(ent, prop->GetDataTable(), depth + 1, ft, acc_offset + prop->GetOffset());
		}
		if (forcetable && strcmp(ft, table->GetName())) continue;
		switch (prop->GetType()) {
		case SendPropType::DPT_Float:
			logging::Info("%s [0x%04x] = %f", prop->GetName(), prop->GetOffset(), CE_FLOAT(ent, acc_offset + prop->GetOffset()));
		break;
		case SendPropType::DPT_Int:
			logging::Info("%s [0x%04x] = %i | %u | %hd | %hu", prop->GetName(), prop->GetOffset(), CE_INT(ent, acc_offset + prop->GetOffset()), CE_VAR(ent, acc_offset +  prop->GetOffset(), unsigned int), CE_VAR(ent, acc_offset + prop->GetOffset(), short), CE_VAR(ent, acc_offset + prop->GetOffset(), unsigned short));
		break;
		case SendPropType::DPT_String:
			logging::Info("%s [0x%04x] = %s", prop->GetName(), prop->GetOffset(), CE_VAR(ent, prop->GetOffset(), char*));
		break;
		case SendPropType::DPT_Vector:
			logging::Info("%s [0x%04x] = (%f, %f, %f)", prop->GetName(), prop->GetOffset(), CE_FLOAT(ent, acc_offset + prop->GetOffset()), CE_FLOAT(ent, acc_offset + prop->GetOffset() + 4), CE_FLOAT(ent, acc_offset + prop->GetOffset() + 8));
		break;
		case SendPropType::DPT_VectorXY:
			logging::Info("%s [0x%04x] = (%f, %f)", prop->GetName(), prop->GetOffset(), CE_FLOAT(ent, acc_offset + prop->GetOffset()), CE_FLOAT(ent,acc_offset +  prop->GetOffset() + 4));
		break;
		}

	}
	if (!ft || !strcmp(ft, table->GetName()))
		logging::Info("==== END OF TABLE: %s", table->GetName());
}

void CC_DumpVars(const CCommand& args) {
	if (args.ArgC() < 1) return;
	if (!atoi(args[1])) return;
	int idx = atoi(args[1]);
	CachedEntity* ent = ENTITY(idx);
	if (CE_BAD(ent)) return;
	ClientClass* clz = RAW_ENT(ent)->GetClientClass();
	logging::Info("Entity %i: %s", ent->m_IDX, clz->GetName());
	const char* ft = (args.ArgC() > 1 ? args[2] : 0);
	DumpRecvTable(ent, clz->m_pRecvTable, 0, ft, 0);
}*/

/*void CC_DumpAttribs(const CCommand& args) {
	if (CE_GOOD(g_pLocalPlayer->weapon())) {
		for (int i = 0; i < 15; i++) {
			logging::Info("%i %f", CE_INT(g_pLocalPlayer->weapon(), netvar.AttributeList + i * 12),
					CE_FLOAT(g_pLocalPlayer->weapon(), netvar.AttributeList + i * 12 + 4));
		}
	}
}*/

/*void CC_DumpConds(const CCommand& args) {
	if (args.ArgC() < 1) return;
	if (!atoi(args[1])) return;
	int idx = atoi(args[1]);
	CachedEntity* ent = ENTITY(idx);
	if (CE_BAD(ent)) return;
	if (TF2) {
		condition_data_s d1 = CE_VAR(ent, netvar._condition_bits, condition_data_s);
		logging::Info("0x%08x 0x%08x 0x%08x 0x%08x", d1.cond_0, d1.cond_1, d1.cond_2, d1.cond_3);
	}
	condition_data_s d2 = FromOldNetvars(ent);
	logging::Info("0x%08x 0x%08x 0x%08x 0x%08x", d2.cond_0, d2.cond_1, d2.cond_2, d2.cond_3);
}*/

/*Misc::Misc() {
	if (TF2C) v_bMinigunJump = new CatVar(CV_SWITCH, "minigun_jump", "0", "Minigun Jump", NULL, "Allows you to jump while with minigun spun up");
	v_bDebugInfo = new CatVar(CV_SWITCH, "misc_debug", "0", "Debug info", NULL, "Log stuff to console, enable this if tf2 crashes");
	c_Name = CreateConCommand(CON_PREFIX "name", CC_SetName, "Sets custom name");
	if (TF2) c_DumpItemAttributes = CreateConCommand(CON_PREFIX "dump_item_attribs", CC_DumpAttribs, "Dump active weapon attributes");
	v_bAntiAFK = new CatVar(CV_SWITCH, "noafk", "0", "Anti AFK", NULL, "Sends random stuff to server to not be kicked for idling");
	c_SayLine = CreateConCommand(CON_PREFIX "say_lines", CC_SayLines, "Uses ^ as a newline character");
	c_Shutdown = CreateConCommand(CON_PREFIX "shutdown", CC_Shutdown, "Stops the hack");
	c_AddFriend = CreateConCommand(CON_PREFIX "addfriend", CC_AddFriend, "Adds a friend");
	c_AddRage = CreateConCommand(CON_PREFIX "addrage", CC_AddRage, "Adds player to rage list");
	c_DumpVars = CreateConCommand(CON_PREFIX "dumpent", CC_DumpVars, "Dumps entity data");
	c_DumpPlayers = CreateConCommand(CON_PREFIX "dumpplayers", CC_DumpPlayers, "Dumps player data");
	c_DumpConds = CreateConCommand(CON_PREFIX "dumpconds", CC_DumpConds, "Dumps conditions");
	c_Teamname = CreateConCommand(CON_PREFIX "teamname", CC_Teamname, "Team name");
	c_Lockee = CreateConCommand(CON_PREFIX "lockee", CC_Lockee, "Lock/Unlock commands");
	c_Reset = CreateConCommand(CON_PREFIX "reset_lists", CC_ResetLists, "Remove all friends and rage");
	c_Disconnect = CreateConCommand(CON_PREFIX "disconnect", CC_Disconnect, "Disconnect");
	c_DisconnectVAC = CreateConCommand(CON_PREFIX "disconnect_vac", CC_Misc_Disconnect_VAC, "Disconnect (VAC)");
	v_bInfoSpam = CreateConVar(CON_PREFIX "info_spam", "0", "Info spam");
	v_bFastCrouch = CreateConVar(CON_PREFIX "fakecrouch", "0", "Fast crouch");
	v_bFlashlightSpam = new CatVar(CV_SWITCH, "flashlight_spam", "0", "Flashlight Spam", NULL, "Quickly turns flashlight on and off");
	v_iFakeLag = new CatVar(CV_INT, "fakelag", "0", "Fakelag", NULL, "# of packets jammed", true, 25.0f);
	c_Unrestricted = CreateConCommand(CON_PREFIX "cmd", CC_Unrestricted, "Execute a ConCommand");
	c_SaveSettings = CreateConCommand(CON_PREFIX "save", CC_SaveConVars, CON_PREFIX "save [file]\nSave settings to cfg/cat_[file].cfg, file is lastcfg by default\n");
	//v_bDumpEventInfo = CreateConVar(CON_PREFIX "debug_event_info", "0", "Show event info");
	CreateConCommand(CON_PREFIX "set", CC_SetValue, "Set ConVar value (if third argument is 1 the ^'s will be converted into newlines)");
	if (TF2C) v_bTauntSlide = new CatVar(CV_SWITCH, "tauntslide", "0", "Taunt Slide", NULL, "Works only in TF2 Classic!");
	if (TF) v_bCritHack = new CatVar(CV_SWITCH, "crits", "0", "Crit Hack", NULL, "BindToggle that to a key, while enabled, you can only shoot criticals. Be careful not to exhaust the crit bucket!");
	//v_bDebugCrits = new CatVar(CV_SWITCH, "debug_crits", "0", "Debug Crits", NULL, "???");
	v_bCleanChat = new CatVar(CV_SWITCH, "clean_chat", "1", "Remove newlines from messages", NULL, "Removes newlines from messages, at least it should do that. Might be broken.");
	if (TF2) c_Schema = CreateConCommand(CON_PREFIX "schema", CC_Misc_Schema, "Load item schema");
	if (TF) v_bDebugCrits = new CatVar(CV_SWITCH, "debug_crits", "0", "???", NULL, "???");
	if (TF) v_bSuppressCrits = new CatVar(CV_SWITCH, "suppress_crits", "1", "Suppress non-forced crits", NULL, "Helps to save the crit bucket");
	//if (TF2) v_bHookInspect = new CatVar(CV_SWITCH, "hook_inspect", "0", "Hook CanInspect", NULL, "Once enabled, can't be turned off. cathook can't be unloaded after enabling it");
	//eventManager->AddListener(&listener, "player_death", false);
}*/
