#include "../Commands/Settings.h"

#include "../../ESPEasy_common.h"

#include "../Commands/Common.h"

#include "../ESPEasyCore/ESPEasyNetwork.h"
#include "../ESPEasyCore/Serial.h"

#include "../Globals/SecuritySettings.h"
#include "../Globals/Settings.h"
#include "../Globals/Groups.h"

#include "../Helpers/ESPEasy_FactoryDefault.h"
#include "../Helpers/ESPEasy_Storage.h"
#include "../Helpers/Memory.h"
#include "../Helpers/Misc.h"
#include "../Helpers/StringConverter.h"
#include "../Helpers/StringParser.h"


String Command_Settings_Build(struct EventStruct *event, const char* Line)
{
	if (HasArgv(Line, 2)) {
		Settings.Build = event->Par1;
	} else {
		serialPrintln();
		String result = F("Build:");
		result += Settings.Build;
    return return_result(event, result);
	}
	return return_command_success();
}

String Command_Settings_Unit(struct EventStruct *event, const char* Line)
{
	if (HasArgv(Line, 2)) {
		Settings.Unit = event->Par1;
	}else  {
		serialPrintln();
		String result = F("Unit:");
		result += Settings.Unit;
    return return_result(event, result);
	}
	return return_command_success();
}

String Command_Settings_Name(struct EventStruct *event, const char* Line)
{
	return Command_GetORSetString(event, F("Name:"),
				      Line,
				      Settings.Name,
				      sizeof(Settings.Name),
				      1);
}

String Command_GroupName(struct EventStruct *event, const char* Line)
{
  	unsigned const group = event->Par1;
  	if (group < 8)
  	{
		return Command_GetORSetString(event, F("GroupName:"),
						Line,
						GroupInfos[group].name,
						sizeof(GroupInfos[group].name),
						2);
  	}
  	return return_command_failed();
}

String Command_GroupSet(struct EventStruct *event, const char* Line)
{
  	int const group = event->Par1;
  	if ((unsigned)group < 8) {
		const uint8_t mask = (1 << group);
		if (event->Par2 < 0) {// -1 -> delete group
			for(unsigned i=0; i<sizeof(Groups); i++)
				Groups[i] = Groups[i] &~ mask;
		} else {
			const uint8_t unit = event->Par2;
			const unsigned enable = event->Par3; // bit0: M1, bit1: M2
			Groups[unit] = (Groups[unit] &~ mask) | ((enable&1)<<group);
			Groups[(int)unit+256] = (Groups[(int)unit+256] &~ mask) | (((enable>>1)&1)<<group);
		}
	  	return return_command_success();
  	}
  	return return_command_failed();
}

String Command_Settings_Password(struct EventStruct *event, const char* Line)
{
	return Command_GetORSetString(event, F("Password:"),
				      Line,
				      SecuritySettings.Password,
				      sizeof(SecuritySettings.Password),
				      1
				      );
}

String Command_Settings_Save(struct EventStruct *event, const char* Line)
{
	SaveSettings();
	SaveGroupSettings();
	return return_command_success();
}

String Command_Settings_Load(struct EventStruct *event, const char* Line)
{
	LoadSettings();
	return return_command_success();
}

String Command_Settings_Print(struct EventStruct *event, const char* Line)
{
	serialPrintln();

	serialPrintln(F("System Info"));
	serialPrint(F("  IP Address    : ")); serialPrintln(NetworkLocalIP().toString());
	serialPrint(F("  Build         : ")); serialPrintln(String((int)BUILD));
	serialPrint(F("  Name          : ")); serialPrintln(Settings.Name);
	serialPrint(F("  Unit          : ")); serialPrintln(String((int)Settings.Unit));
	serialPrint(F("  WifiSSID      : ")); serialPrintln(SecuritySettings.WifiSSID);
	serialPrint(F("  WifiKey       : ")); serialPrintln(SecuritySettings.WifiKey);
	serialPrint(F("  WifiSSID2     : ")); serialPrintln(SecuritySettings.WifiSSID2);
	serialPrint(F("  WifiKey2      : ")); serialPrintln(SecuritySettings.WifiKey2);
	serialPrint(F("  Free mem      : ")); serialPrintln(String(FreeMem()));
	return return_see_serial(event);
}

String Command_Settings_Reset(struct EventStruct *event, const char* Line)
{
	ResetFactory();
	reboot(ESPEasy_Scheduler::IntendedRebootReason_e::ResetFactoryCommand);
	return return_command_success();
}
