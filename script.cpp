/*d
Code adapted from GTA V SCRIPT HOOK SDK example found at (http://dev-c.com	(C) Alexander Blade 2015)
*/


#include <string>
#include <vector>
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>

#include "script.h"
#include "keyboard.h"

#include <string>
#include <ctime>

#pragma warning(disable : 4244 4305) // double <-> float conversions

void draw_rect(float A_0, float A_1, float A_2, float A_3, int A_4, int A_5, int A_6, int A_7)
{
	GRAPHICS::DRAW_RECT((A_0 + (A_2 * 0.5f)), (A_1 + (A_3 * 0.5f)), A_2, A_3, A_4, A_5, A_6, A_7);
}
float sp = 0;
float wattage=0;
float gospeed;

float el;
void draw_menu_line(std::string caption, float lineWidth, float lineHeight, float lineTop, float lineLeft, float textLeft, bool active, bool title, bool rescaleText = true, float textOR = 0)
{
	// default values
	int text_col[4] = {255, 255, 255, 255},
		rect_col[4] = {70, 95, 95, 200};
	float text_scale = 0.35;
	int font = 0;

	// correcting values for active line
	if (active)
	{
		text_col[0] = 0;
		text_col[1] = 0;
		text_col[2] = 0;

		rect_col[0] = 218;
		rect_col[1] = 242;
		rect_col[2] = 216;

		if (rescaleText) text_scale = 0.40;
	}

	if (title)
	{
		rect_col[0] = 0;
		rect_col[1] = 0;
		rect_col[2] = 0;

		if (rescaleText) text_scale = 0.50;
		font = 1;
	}

	if (textOR != 0)
		text_scale = textOR;

	int screen_w, screen_h;
	GRAPHICS::GET_SCREEN_RESOLUTION(&screen_w, &screen_h);

	textLeft += lineLeft;

	float lineWidthScaled = lineWidth / (float)screen_w; // line width
	float lineTopScaled = lineTop / (float)screen_h; // line top offset
	float textLeftScaled = textLeft / (float)screen_w; // text left offset
	float lineHeightScaled = lineHeight / (float)screen_h; // line height

	float lineLeftScaled = lineLeft / (float)screen_w;

	// this is how it's done in original scripts

	// text upper part
	UI::SET_TEXT_FONT(font);
	UI::SET_TEXT_SCALE(0.0, text_scale);
	UI::SET_TEXT_COLOUR(text_col[0], text_col[1], text_col[2], text_col[3]);
	UI::SET_TEXT_CENTRE(0);
	UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
	UI::SET_TEXT_EDGE(0, 0, 0, 0, 0);
	UI::_SET_TEXT_ENTRY("STRING");
	UI::_ADD_TEXT_COMPONENT_STRING((LPSTR)caption.c_str());
	UI::_DRAW_TEXT(textLeftScaled, (((lineTopScaled + 0.00278f) + lineHeightScaled) - 0.005f));

	// text lower part
	UI::SET_TEXT_FONT(font);
	UI::SET_TEXT_SCALE(0.0, text_scale);
	UI::SET_TEXT_COLOUR(text_col[0], text_col[1], text_col[2], text_col[3]);
	UI::SET_TEXT_CENTRE(0);
	UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
	UI::SET_TEXT_EDGE(0, 0, 0, 0, 0);
	UI::_SET_TEXT_GXT_ENTRY("STRING");
	UI::_ADD_TEXT_COMPONENT_STRING((LPSTR)caption.c_str());
	int num25 = UI::_0x9040DFB09BE75706(textLeftScaled, (((lineTopScaled + 0.00278f) + lineHeightScaled) - 0.005f));

	// rect
	draw_rect(lineLeftScaled, lineTopScaled + (0.00278f), 
		lineWidthScaled, ((((float)(num25)* UI::_0xDB88A37483346780(text_scale, 0)) + (lineHeightScaled * 2.0f)) + 0.005f),
		rect_col[0], rect_col[1], rect_col[2], rect_col[3]);	
}

bool trainer_switch_pressed()
{
	return IsKeyJustUp(VK_F4);
}

void get_button_state(bool *a, bool *b, bool *up, bool *down, bool *l, bool *r)
{
	if (a) *a = IsKeyDown(VK_NUMPAD5);
	if (b) *b = IsKeyDown(VK_NUMPAD0) || trainer_switch_pressed() || IsKeyDown(VK_BACK);
	if (up) *up = IsKeyDown(VK_NUMPAD8);
	if (down) *down = IsKeyDown(VK_NUMPAD2);
	if (r) *r = IsKeyDown(VK_NUMPAD6);
	if (l) *l = IsKeyDown(VK_NUMPAD4);
}

void menu_beep()
{
	AUDIO::PLAY_SOUND_FRONTEND(-1, "NAV_UP_DOWN", "HUD_FRONTEND_DEFAULT_SOUNDSET", 0);
}

std::string statusText;
DWORD statusTextDrawTicksMax;
bool statusTextGxtEntry;

void update_status_text()
{
	if (GetTickCount() < statusTextDrawTicksMax)
	{
		UI::SET_TEXT_FONT(0);
		UI::SET_TEXT_SCALE(0.55, 0.55);
		UI::SET_TEXT_COLOUR(255, 255, 255, 255);
		UI::SET_TEXT_WRAP(0.0, 1.0);
		UI::SET_TEXT_CENTRE(1);
		UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
		UI::SET_TEXT_EDGE(1, 0, 0, 0, 205);
		if (statusTextGxtEntry)
		{
			UI::_SET_TEXT_ENTRY((char *)statusText.c_str());
		} else
		{
			UI::_SET_TEXT_ENTRY("STRING");
			UI::_ADD_TEXT_COMPONENT_STRING((char *)statusText.c_str());
		}
		UI::_DRAW_TEXT(0.5, 0.5);
	}
}

void set_status_text(std::string str, DWORD time = 2500, bool isGxtEntry = false)
{
	statusText = str;
	statusTextDrawTicksMax = GetTickCount() + time;
	statusTextGxtEntry = isGxtEntry;
}

// features
bool featurePlayerInvincible			=	false;
bool featurePlayerInvincibleUpdated		=	false;
bool featurePlayerNeverWanted			=	false;
bool featurePlayerIgnored				=	false;
bool featurePlayerIgnoredUpdated		=	false;
bool featurePlayerUnlimitedAbility		=	false;
bool featurePlayerNoNoise				=	false;
bool featurePlayerNoNoiseUpdated		=	false;
bool featurePlayerFastSwim				=	false;
bool featurePlayerFastSwimUpdated		=	false;
bool featurePlayerFastRun				=	false;
bool featurePlayerFastRunUpdated		=	false;
bool featurePlayerSuperJump				=	false;

bool featureWeaponNoReload				=	false;
bool featureWeaponFireAmmo				=	false;
bool featureWeaponExplosiveAmmo			=	false;
bool featureWeaponExplosiveMelee		=	false;
bool featureWeaponVehRockets			=	false;

DWORD featureWeaponVehShootLastTime		=	0;

bool featureVehInvincible				=	false;
bool featureVehInvincibleUpdated		=	false;
bool featureVehInvincibleWheels			=	false;
bool featureVehInvincibleWheelsUpdated	=	false;
bool featureVehSeatbelt					=	false;
bool featureVehSeatbeltUpdated			=	false;
bool featureVehSpeedBoost				=	false;
bool featureVehWrapInSpawned			=	false;

bool featureWorldMoonGravity			=	false;
bool featureWorldRandomCops				=	true;
bool featureWorldRandomTrains			=	true;
bool featureWorldRandomBoats			=	true;
bool featureWorldGarbageTrucks			=	true;

bool featureTimePaused					=	false;
bool featureTimePausedUpdated			=	false;
bool featureTimeSynced					=	false;

bool featureWeatherWind					=	false;
bool featureWeatherPers					=	false;

bool featureMiscLockRadio				=	false;
bool featureMiscHideHud					=	false;


/**** SLOT ****/
HANDLE hSlot;
LPTSTR SlotName = TEXT("\\\\.\\mailslot\\sample_mailslot");
LPTSTR lpszBuffer;
int latestValue = -1;
float targetSpeed = 0;
bool failure = false;
float tick = 0;
float pd = 70;
std::vector<float> t;
BOOL ReadSlot()
{
	DWORD cbMessage, cMessage, cbRead;
	BOOL fResult;

	TCHAR achID[80];
	DWORD cAllMessages;
	HANDLE hEvent;
	OVERLAPPED ov;

	cbMessage = cMessage = cbRead = 0;

	hEvent = CreateEvent(NULL, FALSE, FALSE, TEXT("ExampleSlot"));
	if (NULL == hEvent)
		return FALSE;
	ov.Offset = 0;
	ov.OffsetHigh = 0;
	ov.hEvent = hEvent;

	fResult = GetMailslotInfo(hSlot, // mailslot handle 
		(LPDWORD)NULL,               // no maximum message size 
		&cbMessage,                   // size of next message 
		&cMessage,                    // number of messages 
		(LPDWORD)NULL);              // no read time-out 

	if (!fResult)
	{
		printf("GetMailslotInfo failed with %d.\n", GetLastError());
		return FALSE;
	}

	if (cbMessage == MAILSLOT_NO_MESSAGE)
	{
		printf("Waiting for a message...\n");
		return TRUE;
	}

	cAllMessages = cMessage;

	while (cMessage != 0)  // retrieve all messages
	{
		// Create a message-number string. 



		// Allocate memory for the message. 

		lpszBuffer = (LPTSTR)GlobalAlloc(GPTR,
			lstrlen((LPTSTR)achID)*sizeof(TCHAR) + cbMessage);
		if (NULL == lpszBuffer)
			return FALSE;
		lpszBuffer[0] = '\0';

		fResult = ReadFile(hSlot,
			lpszBuffer,
			cbMessage,
			&cbRead,
			NULL);

		if (!fResult)
		{
			printf("ReadFile failed with %d.\n", GetLastError());
			GlobalFree((HGLOBAL)lpszBuffer);
			return FALSE;
		}

		// Concatenate the message and the message-number string. 
		latestValue = _ttoi(lpszBuffer);

		/*
		StringCbCat(lpszBuffer,sesnor
		lstrlen((LPTSTR)achID)*sizeof(TCHAR) + cbMessage,
		(LPTSTR)achID);**/

		// Display the message. 

		_tprintf(TEXT("Contents of the mailslot: %s\n"), lpszBuffer);

		GlobalFree((HGLOBAL)lpszBuffer);

		fResult = GetMailslotInfo(hSlot,  // mailslot handle 
			(LPDWORD)NULL,               // no maximum message size 
			&cbMessage,                   // size of next message 
			&cMessage,                    // number of messages 
			(LPDWORD)NULL);              // no read time-out 

		if (!fResult)
		{
			printf("GetMailslotInfo failed (%d)\n", GetLastError());
			return FALSE;
		}
	}
	CloseHandle(hEvent);
	return TRUE;
}
float lt = 0;
BOOL WINAPI MakeSlot(LPTSTR lpszSlotName)
{
	
	hSlot = CreateMailslot(lpszSlotName,
		0,                             // no maximum message size 
		MAILSLOT_WAIT_FOREVER,         // no time-out for operations 
		(LPSECURITY_ATTRIBUTES)NULL); // defaul


	if (hSlot == INVALID_HANDLE_VALUE)
	{	
		char caption[128];
		sprintf_s(caption, "CreateMailslot failed with %d\n", GetLastError());
		draw_menu_line(caption, 350.0, 15.0, 18.0, 800.0, 5.0, false, true);
		
		return FALSE;
	}
	return TRUE;
}
bool madeSlot = false;


/**** END SLOT ****/

// player model control, switching on normal ped model when needed	
void check_player_model() 
{
	// common variables
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();	

	if (!ENTITY::DOES_ENTITY_EXIST(playerPed)) return;

	Hash model = ENTITY::GET_ENTITY_MODEL(playerPed);
	if (ENTITY::IS_ENTITY_DEAD(playerPed) || PLAYER::IS_PLAYER_BEING_ARRESTED(player, TRUE))
		if (model != GAMEPLAY::GET_HASH_KEY("player_zero") && 
			model != GAMEPLAY::GET_HASH_KEY("player_one") &&
			model != GAMEPLAY::GET_HASH_KEY("player_two"))
		{
			set_status_text("turning to normal");
			WAIT(1000);

			model = GAMEPLAY::GET_HASH_KEY("player_zero");
			STREAMING::REQUEST_MODEL(model);
			while (!STREAMING::HAS_MODEL_LOADED(model))
				WAIT(0);
			PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model);
			PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);

			// wait until player is ressurected
			while (ENTITY::IS_ENTITY_DEAD(PLAYER::PLAYER_PED_ID()) || PLAYER::IS_PLAYER_BEING_ARRESTED(player, TRUE))
				WAIT(0);

		}
}

void Unreg() {
	CloseHandle(hSlot);
}
void update_vehicle_guns()
{
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();	

	if (!ENTITY::DOES_ENTITY_EXIST(playerPed) || !featureWeaponVehRockets) return;

	bool bSelect = IsKeyDown(0x6B); // num plus
	if (bSelect && featureWeaponVehShootLastTime + 150 < GetTickCount() &&
		PLAYER::IS_PLAYER_CONTROL_ON(player) &&	PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);

		Vector3 v0, v1;
		GAMEPLAY::GET_MODEL_DIMENSIONS(ENTITY::GET_ENTITY_MODEL(veh), &v0, &v1);
		
		Hash weaponAssetRocket = GAMEPLAY::GET_HASH_KEY("WEAPON_VEHICLE_ROCKET");
		if (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
		{
			WEAPON::REQUEST_WEAPON_ASSET(weaponAssetRocket, 31, 0);
			while (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
				WAIT(0);
		}

		Vector3 coords0from = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, -(v1.x + 0.25f), v1.y + 1.25f, 0.1);
		Vector3 coords1from = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh,  (v1.x + 0.25f), v1.y + 1.25f, 0.1);
		Vector3 coords0to = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, -v1.x, v1.y + 100.0f, 0.1f);
		Vector3 coords1to = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh,  v1.x, v1.y + 100.0f, 0.1f);

		GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords0from.x, coords0from.y, coords0from.z, 
													 coords0to.x, coords0to.y, coords0to.z, 
													 250, 1, weaponAssetRocket, playerPed, 1, 0, -1.0);
		GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords1from.x, coords1from.y, coords1from.z, 
													 coords1to.x, coords1to.y, coords1to.z, 
													 250, 1, weaponAssetRocket, playerPed, 1, 0, -1.0);

		featureWeaponVehShootLastTime = GetTickCount();
	}
}
void doBikeInterface();
void 	doAnyCar(); 
bool skinchanger_used = false;

float vlen(Vector3 fv) {
	return sqrt(fv.x*fv.x + fv.y*fv.y + fv.z*fv.z);
}
Vector3 norm(Vector3 fv) {

	float norm = sqrt(fv.x*fv.x + fv.y*fv.y + fv.z*fv.z);
	Vector3 out; 
	out.x = fv.x / norm;
	out.y = fv.y / norm;
	out.z = fv.z / norm;
	return fv;
}
bool dev = false;
bool funSpeed = true,original,allVeh = true;
float target = 200;
int mode = 0;
int MAXMODE = 3;
// Updates all features that can be turned off by the game, being called each game frame
void update_features() 
{
	update_status_text();

	

	// wait until player is ready, basicly to prevent using the trainer while player is dead or arrested
	while (ENTITY::IS_ENTITY_DEAD(PLAYER::PLAYER_PED_ID()) || PLAYER::IS_PLAYER_BEING_ARRESTED(PLAYER::PLAYER_ID(), TRUE))
		WAIT(0);

	// read default feature values from the game
	featureWorldRandomCops = PED::CAN_CREATE_RANDOM_COPS() == TRUE;

	// common variable


	if (!madeSlot && !dev) {
		madeSlot = MakeSlot(SlotName);
	}
	/*TRAINER*/

	if (IsKeyJustUp(VK_F9)) {
		funSpeed = !funSpeed;
	}

	if (IsKeyJustUp(VK_F12)) {
		allVeh = !allVeh;
	}



	if (dev) {
		if (IsKeyJustUp(VK_F10)) {
			wattage += 10;
		}

		if (IsKeyJustUp(VK_F11)) {
			wattage -= 10;
			wattage = max(0, wattage);

		}

		

	}
	if (IsKeyJustUp(VK_F8)) {
		sp = 0;
		mode = (mode + 1) % MAXMODE;
		if (mode == 0) {

			dev = !dev;
		}
	}
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (mode == 0) {
		if (PED::IS_PED_ON_ANY_BIKE(playerPed))
			doBikeInterface();
		else
			doAnyCar();
	}
	else if (mode == 1) {

		if (PED::IS_PED_ON_ANY_BIKE(playerPed))
			doBikeInterface();
	}
	else {
	
	}


}

void doAnyCar() {
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(playerPed);
	
	if (bPlayerExists)
	{

		if (tick == 0) tick = GetTickCount();
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
		DWORD model = ENTITY::GET_ENTITY_MODEL(veh);
		int dv = CONTROLS::GET_CONTROL_VALUE(2, 10);
		


		if (dev || ReadSlot()) {
			if (!dev) {
				wattage = (5.244820) * (0.001*0.6213*latestValue) + (0.019168) * (0.001*0.6213*latestValue) * (0.001*0.6213*latestValue)* (0.001*0.6213*latestValue);
			}
			float p = (wattage / target);
			bool bDown = CONTROLS::IS_CONTROL_PRESSED(2, 10);

			if (PED::IS_PED_IN_ANY_VEHICLE(playerPed,false)) {	
				Vector3 v = ENTITY::GET_ENTITY_VELOCITY(veh);

				if (p > 1) {
					p = sqrt(sqrt(p));
				}
				if(!bDown && VEHICLE::IS_VEHICLE_ON_ALL_WHEELS(veh))
					ENTITY::SET_ENTITY_VELOCITY(veh, v.x*.99 + .01*v.x*p, v.y*.99 + .01*v.y*p, v.z);
				char caption[1000];
				sprintf_s(caption, "w: %f s: %0.2f %0.2f s:%0.2f a: %d %0.4f  zz", wattage, vlen(v), ENTITY::GET_ENTITY_SPEED(veh), bDown, v.y, v.z);
				draw_menu_line(caption, 350.0, 15.0, 18.0, 800.0, 5.0, false, true);
				
			}	
			else {
				Vector3 pos = ENTITY::GET_ENTITY_COORDS(player, true);
				Vector3 hadap = ENTITY::GET_ENTITY_FORWARD_VECTOR(playerPed);
			
				float s = ENTITY::GET_ENTITY_SPEED(playerPed);
				Vector3 n = norm(hadap);
				float scale = 0;
				if (p <= 1) {
					scale = (p - 1)*s*0.25;
					ENTITY::APPLY_FORCE_TO_ENTITY(playerPed, 1, hadap.x * scale, hadap.y * scale, hadap.z * scale, 0, 0, 0, 1, false, true, true, true, true);
				}
				else if (s >= 0) {
					if (s > 1) {
						scale = ((p-1) / s)*5;
						if (CONTROLS::IS_CONTROL_PRESSED(2, 32))
							ENTITY::APPLY_FORCE_TO_ENTITY(playerPed, 1, n.x * scale, n.y * scale, n.z * scale, 0, 0, 0, 1, false, true, true, true, true);
					}	
					//PLAYER::_SET_MOVE_SPEED_MULTIPLIER(player, min(p,1.5));
				}
				
			
				char caption[1000];
				sprintf_s(caption, "w: %f %f s: %0.2f %d ", wattage,s, hadap.x * scale,  CONTROLS::IS_CONTROL_PRESSED(2, 32));
				draw_menu_line(caption, 350.0, 15.0, 18.0, 800.0, 5.0, false, true);



			}

		}
		else {
			char caption[100];
			if (madeSlot)
				sprintf_s(caption, "No sensor data!");

			//sprintf_s(caption, "Sensor creation failed");
			draw_menu_line(caption, 350.0, 15.0, 18.0, 800.0, 5.0, false, true);
		}


	}
	else {
		tick = 0;
		sp = 0;

	}

}

void doBikeInterface() {
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(playerPed);
	if (bPlayerExists && (allVeh || PED::IS_PED_ON_ANY_BIKE(playerPed) ))
	{
		if (tick == 0) tick = GetTickCount();
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
		DWORD model = ENTITY::GET_ENTITY_MODEL(veh);

		
		bool bDown = IsKeyDown(VK_DOWN); 
		
		
		if (dev || ReadSlot()) {

			int dv = CONTROLS::GET_CONTROL_VALUE(2, 10);
			if (bDown) {

				dv = 254;
			}
			el = ENTITY::GET_ENTITY_PITCH(playerPed) / 100;

				
			if (!dev) {
				wattage = (5.244820) * (0.001*0.6213*latestValue) + (0.019168) * (0.001*0.6213*latestValue) * (0.001*0.6213*latestValue)* (0.001*0.6213*latestValue);
			}



			float wind2 = 1.226*0.5*0.5*(sp*sp)*0.5;
			float frict2 = 75 * 9.8* 0.004;
			float gravity2 = 75 * 9.8* el;
			float resist2 = wind2 + frict2 + gravity2;


			float accel = 0;
			float scale = (GetTickCount() - tick) / 1000;
			if (scale > 0.125)
				scale = 0.125;
			tick = GetTickCount();
			float lp = 0;


			float press = ((dv - 127.0) / (254 - 127));
			if (sp > 0) {
				accel = (wattage / (75 * sp) - (resist2 / 75)) * 1;
				accel -= press * 5;
			}
			else {
				accel = (wind2 / 75 + frict2 / 25 + wattage / 75 - gravity2 / 75) * 1;
				accel += press * 5;
			}

			sp = (sp + accel*scale);//+ 0.065;
			int mod = 1;
			/*if(funSpeed )
				mod *= 2;*/
			if (VEHICLE::IS_VEHICLE_ON_ALL_WHEELS(veh))
				VEHICLE::SET_VEHICLE_FORWARD_SPEED(veh, sp*mod);

			char caption[1000];
			if (dev) {
				sprintf_s(caption, "w: %f s: %0.2f s:%0.2f a: %0.4f e:%0.2f r:%0.2f", wattage, sp, scale, accel, el, lp);
				draw_menu_line(caption, 350.0, 15.0, 18.0, 800.0, 5.0, false, true);
			}
			else {

				sprintf_s(caption, "Power: %5.2f | Speed (sensor): %5.2f | Speed(world) : %6.2f | Elev: % +6.2f", wattage, (0.001*0.6213*latestValue), ((sp * 3600) / 1000)*(0.6213), el * 100);

				draw_menu_line(caption, 800.0, 20.0, 0, 0, 5.0, false, false, true, 0.5);
				/*
				sprintf_s(caption, "Power: %0.2f", wattage );
				draw_menu_line(caption, 200.0, 15.0, 18.0 , 800.0, 5.0, false, true, true, 0.6); 
				
				sprintf_s(caption, "Speed (sensor): %0.2f", (0.001*0.6213*latestValue));
				draw_menu_line(caption, 200.0, 15.0, 18.0+30*1, 800.0, 5.0, false, true,true,0.6);
				
				if (funSpeed) {
					sprintf_s(caption, "Speed (funworld) : %0.2f", ((sp * 3600) / 1000)*(0.6213));
				}
				else {
					sprintf_s(caption, "Speed (world) : %0.2f", ((sp * 3600) / 1000)*(0.6213));
				}
				draw_menu_line(caption, 200.0, 15.0, 18.0 +30 * 2, 800.0, 5.0, false, true, true, 0.6); 
				sprintf_s(caption, "Elev: %0.1f", el * 100);
				draw_menu_line(caption, 200.0, 15.0, 18.0 + 30 * 3, 800.0, 5.0, false, true, true, 0.6); 
				
				*/
			}

	
			//float gradedLatest = max(latestValue + (el*100)*-3720.0,0);
			//targetSpeed = (latestValue *2.0/ 3600)*(alpha)+(1 - alpha)*targetSpeed*(1-((dv-127.0)/254));
			//targetSpeed = (speed + ((max(0,latestValue-MetersPerHour)) *2.0/ 3600))*(1-((dv-127.0)/254));

			//			float norm = sqrt(fv.x*fv.x + fv.y*fv.y + fv.z*fv.z);
			//float scale = (latestValue / pd)*.2;



			/*
			DWORD maxTickCount = GetTickCount() + tick;
			do
			{

			speed = ENTITY::GET_ENTITY_SPEED(veh);

			/*
			if (sp < 0.0125 && sp > -0.0125) {

			if (accel >= 0) {
			lp = sp = 0.0125;
			}
			else {


			lp = sp = -0.0125;
			}
			}

			//Vector3 v = ENTITY::GET_ENTITY_VELOCITY(veh);
			//float norm = sqrt(v.x*fv.x + v.y*v.y + v.z*v.z);

			//float scale = (latestValue / pd)*.2;
			//sprintf_s(caption, "m/hr: %d a: %0.2f M:%0.2f Grade: %0.2f E:%0.2f", latestValue, wattage, MetersPerHour, pd, 0);

			char caption[1000];
			sprintf_s(caption, "m/hr: %f w: %0.2f GS:%0.2f R: %0.2f GR:%0.2f ", wattage, accel, targetSpeed, speed, el);
			draw_menu_line(caption, 350.0, 15.0, 18.0, 800.0, 5.0, false, true);
			WAIT(0);
			} while (GetTickCount() < maxTickCount);
			*/

			//	if (!ENTITY::IS_ENTITY_IN_AIR(veh))
			//	ENTITY::APPLY_FORCE_TO_ENTITY(veh, 1, scale*nvf.x,  scale*nvf.y , scale*nvf.z, 0.0f, 0.0f, 0.0f, true, false, true, true, true, true);

			//lt = GetTickCount() - (maxTickCount - tick);

			// display

			///gospeed = wattage / resist;
			/*
			float hi = 100;
			float low = 0;
			float guess = 0;
			if (resist < 0) {
			for (int i = 0; i < 10; i++) {
			float guess = (hi - low) / 2 + low;
			wind = 1.226*0.5*0.5*0.5*(guess*guess);
			resist = wind + frict + gravity;
			float powerguess = speed * resist;

			if (wattage > powerguess)
			low = guess;
			else
			hi = guess;

			}
			gospeed = (hi - low) / 2 + low;

			}
			*/


		}
		else {
			char caption[100];
			if (madeSlot)
				sprintf_s(caption, "No sensor data!");

			sprintf_s(caption, "Sensor creation failed");
			draw_menu_line(caption, 350.0, 15.0, 18.0, 800.0, 5.0, false, true);
		}


	}
	else {
		tick = 0;
		sp = 0;

	}

}

float calcSpeed(float spz) {
	float wind2 = 1.226*0.5*0.5*(spz*spz)*0.5;
	float frict2 = 75 * 9.8* 0.004;
	float gravity2 = 75 * 9.8* el;
	float resist2 = wind2 + frict2 + gravity2;


	float lp = 0;

	float a;
	//float press = ((dv - 127.0) / (254 - 127));
	if (spz > 0) {
		a = (wattage / (75 * spz) - (resist2 / 75)) * 1;
	
	}
	else {
		a = (wind2 / 75 + frict2 / 25 + wattage / 75 - gravity2 / 75) * 1;

	}
	return a;
}

void doBikeInterface2() {
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(playerPed);
	if (bPlayerExists && PED::IS_PED_ON_ANY_BIKE(playerPed))
	{
		if (tick == 0) tick = GetTickCount();
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
		DWORD model = ENTITY::GET_ENTITY_MODEL(veh);

		bool bUp = IsKeyDown(VK_UP) || CONTROLS::IS_CONTROL_PRESSED(2, 11);
		bool bDown = IsKeyDown(VK_DOWN);


		if (dev || ReadSlot()) {


			Vector3 nvf = norm(ENTITY::GET_ENTITY_VELOCITY(veh));
			sp = vlen(ENTITY::GET_ENTITY_VELOCITY(veh));

			int dv = CONTROLS::GET_CONTROL_VALUE(2, 10);
			if (bDown) {

				dv = 254;
			}
			el = ENTITY::GET_ENTITY_PITCH(playerPed) / 100;


			if (!dev) {
				wattage = (5.244820) * (0.001*0.6213*latestValue) + (0.019168) * (0.001*0.6213*latestValue) * (0.001*0.6213*latestValue)* (0.001*0.6213*latestValue);
			}
			float accel = 0;
			float scale = (GetTickCount() - tick) / 1000;
			if (scale > 0.125)
				scale = 0.125;
			tick = GetTickCount();

			float ns = calcSpeed(sp);
			nvf.x = nvf.x + ns;
			nvf.y = nvf.y + ns;
			nvf.z = nvf.z + ns;



			if (!ENTITY::IS_ENTITY_IN_AIR(veh))
				ENTITY::APPLY_FORCE_TO_ENTITY(veh, 1, nvf.x, nvf.y, 0, 0, 0, 0, true, false, false, true, true, true);



			char caption[1000];
			if (dev) {
				sprintf_s(caption, "w: %f s: %0.2f s:%0.2f a: %0.4f e:%0.2f r:%0.2f", wattage, sp, nvf.x, nvf.y, nvf.z, 0);
				draw_menu_line(caption, 350.0, 15.0, 18.0, 800.0, 5.0, false, true);
			}
			else {
				sprintf_s(caption, "Power: %0.2f", wattage);
				draw_menu_line(caption, 200.0, 15.0, 18.0, 800.0, 5.0, false, true, true, 0.6);

				sprintf_s(caption, "Speed (sensor): %0.2f", (0.001*0.6213*latestValue));
				draw_menu_line(caption, 200.0, 15.0, 18.0 + 30 * 1, 800.0, 5.0, false, true, true, 0.6);

				if (funSpeed) {
					sprintf_s(caption, "Speed (world) : %0.2f", ((sp * 3600) / 1000)*(0.6213));
				}
				else {
					sprintf_s(caption, "Speed (fun world) : %0.2f", ((sp * 3600) / 1000)*(0.6213));
				}
				draw_menu_line(caption, 200.0, 15.0, 18.0 + 30 * 2, 800.0, 5.0, false, true, true, 0.6);
				sprintf_s(caption, "Elev: %0.1f", el * 100);
				draw_menu_line(caption, 200.0, 15.0, 18.0 + 30 * 3, 800.0, 5.0, false, true, true, 0.6);


			}

			if (dev) {
				if (IsKeyJustUp(VK_F10)) {
					wattage += 10;
				}

				if (IsKeyJustUp(VK_F11)) {
					wattage -= 10;
					wattage = max(0, wattage);

				}
			}
			//float gradedLatest = max(latestValue + (el*100)*-3720.0,0);
			//targetSpeed = (latestValue *2.0/ 3600)*(alpha)+(1 - alpha)*targetSpeed*(1-((dv-127.0)/254));
			//targetSpeed = (speed + ((max(0,latestValue-MetersPerHour)) *2.0/ 3600))*(1-((dv-127.0)/254));

			//			float norm = sqrt(fv.x*fv.x + fv.y*fv.y + fv.z*fv.z);
			//float scale = (latestValue / pd)*.2;



			/*
			DWORD maxTickCount = GetTickCount() + tick;
			do
			{

			speed = ENTITY::GET_ENTITY_SPEED(veh);

			/*
			if (sp < 0.0125 && sp > -0.0125) {

			if (accel >= 0) {
			lp = sp = 0.0125;
			}
			else {


			lp = sp = -0.0125;
			}
			}

			//Vector3 v = ENTITY::GET_ENTITY_VELOCITY(veh);
			//float norm = sqrt(v.x*fv.x + v.y*v.y + v.z*v.z);

			//float scale = (latestValue / pd)*.2;
			//sprintf_s(caption, "m/hr: %d a: %0.2f M:%0.2f Grade: %0.2f E:%0.2f", latestValue, wattage, MetersPerHour, pd, 0);

			char caption[1000];
			sprintf_s(caption, "m/hr: %f w: %0.2f GS:%0.2f R: %0.2f GR:%0.2f ", wattage, accel, targetSpeed, speed, el);
			draw_menu_line(caption, 350.0, 15.0, 18.0, 800.0, 5.0, false, true);
			WAIT(0);
			} while (GetTickCount() < maxTickCount);
			*/

			//	if (!ENTITY::IS_ENTITY_IN_AIR(veh))
			//	ENTITY::APPLY_FORCE_TO_ENTITY(veh, 1, scale*nvf.x,  scale*nvf.y , scale*nvf.z, 0.0f, 0.0f, 0.0f, true, false, true, true, true, true);

			//lt = GetTickCount() - (maxTickCount - tick);

			// display

			///gospeed = wattage / resist;
			/*
			float hi = 100;
			float low = 0;
			float guess = 0;
			if (resist < 0) {
			for (int i = 0; i < 10; i++) {
			float guess = (hi - low) / 2 + low;
			wind = 1.226*0.5*0.5*0.5*(guess*guess);
			resist = wind + frict + gravity;
			float powerguess = speed * resist;

			if (wattage > powerguess)
			low = guess;
			else
			hi = guess;

			}
			gospeed = (hi - low) / 2 + low;

			}
			*/


		}
		else {
			char caption[100];
			if (madeSlot)
				sprintf_s(caption, "No sensor data!");

			//sprintf_s(caption, "Sensor creation failed");
			draw_menu_line(caption, 350.0, 15.0, 18.0, 800.0, 5.0, false, true);
		}


	}
	else {
		tick = 0;
		sp = 0;

	}

}


void main()
{	
	//reset_globals();

	while (true)
	{
		update_features();		
		WAIT(0);
	}
}

void ScriptMain()
{
	srand(GetTickCount());
	main();
}
