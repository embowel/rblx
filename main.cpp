#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#include <windows.h>

#include "tp_handler/tp_handler.h"
#include "renderer/renderer.h"
#include "cheats/combat/combat.h"
#include "cheats/misc/misc.h"
#include "spotify/spotify.h"

using namespace rbx;

int main() {
	// yes
	SetProcessDPIAware();

	// title noob
	sprintf_s(globals::title, "lilith - #1 | %s", Offsets::ClientVersion.c_str());
	SetConsoleTitle(globals::title);

	// topmost
	auto console = GetConsoleWindow();
	SetWindowPos(console, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

	printf("[*] hi sir.\n");

	{
		if (!mem::get_pid("RobloxPlayerBeta.exe")) {
			printf("[-] robloxplayerbeta.exe not found!\n");
			system("pause");
			return 1;
		}

		if (!mem::get_handle()) {
			printf("[-] failed to get roblox handle!\n");
			system("pause");
			return 1;
		}

		if (!mem::get_base()) {
			printf("[-] failed to get roblox base!\n");
			system("pause");
			return 1;
		}
	}

	printf("[+] robloxplayerbeta.exe found @ 0x%llx\n", mem::rbx_base);

	printf("[*] starting teleport handler\n");
	std::thread teleport_thread(tp_handler::thread);

	sleep_ms(1000);

	printf("[*] starting overlay renderer\n");
	std::thread render_thread([] {
		renderer->initialize();
		});

	sleep_ms(1000);

	printf("[*] starting cache\n");
	std::thread cache_thread(rbx::cache::thread);

	sleep_ms(1000);

	printf("[*] starting combat cheats\n");
	std::thread combat_thread(rbx::cheats::combat::thread);

	sleep_ms(1000);

	printf("[*] starting miscellaneous cheats\n");
	std::thread misc_thread(rbx::cheats::misc::thread);

	/*while (true) {
		system("cls");
		std::string game_name;
		switch (globals::instances::game::curr_game) {
		case supported_game::universal:
			game_name = "Universal";
			break;
		case supported_game::arsenal:
			game_name = "Arsenal";
			break;
		case supported_game::bad_business:
			game_name = "Bad Business";
			break;
		case supported_game::da_hood:
			game_name = "Da Hood";
			break;
		case supported_game::hood_customs:
			game_name = "Hood Customs";
			break;
		case supported_game::phantom_forces:
			game_name = "Phantom Forces";
			break;
		default:
			game_name = "None";
			break;
		}
		printf("[*] current game - %s\n", game_name.c_str());
		sleep_ms(1000);
	}*/

	/*auto h = globals::instances::g_local_player.get_character().find_first_child("Head");
	auto head = part(h.addr);
	head.set_part_transparency(1.f);
	auto f = head.find_first_child("face");
	auto face = part(f.addr);
	face.set_part_transparency(1.f);*/

	while (globals::g_running) {
		if (!FindWindow("WINDOWSCLIENT", nullptr)) globals::g_running = false;
		sleep_ms(250);
	}

	printf("[*] exiting...\n");

	if (render_thread.joinable()) render_thread.join();
	if (teleport_thread.joinable()) teleport_thread.join();
	if (cache_thread.joinable()) cache_thread.join();
	if (combat_thread.joinable()) combat_thread.join();
	if (misc_thread.joinable()) misc_thread.join();

	//system("pause");
	return 0;
}