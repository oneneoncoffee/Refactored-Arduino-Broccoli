#include <ESP8266WiFi.h>
extern "C" {
#include "user_interface.h"
  typedef void (*freedom_outside_cb_t)(uint8 status);
  int wifi_register_send_pkt_freedom_cb(freedom_outside_cb_t cb);
  void wifi_unregister_send_pkt_freedom_cb(void);
  int wifi_send_pkt_freedom(uint8 *buf, int len, bool sys_seq);
}
// run-time variables
char emptySSID[32]; 
uint8_t channelIndex = 0;
uint8_t macAddr[6];
uint8_t wifi_channel = 1;
uint32_t currentTime = 0;
uint32_t packetSize = 0;
uint32_t packetCounter = 0;
uint32_t attackTime = 0;
uint32_t packetRateTime = 0;

uint8_t beaconPacket[109] = {
0x80, 0x00, 0x00, 0x00,             
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
0x00, 0x00, 0x83, 0x51, 0xf7, 
0x8f, 0x0f, 0x00, 0x00, 0x00, 0x64, 0x03,                         
0x31, 0x00, 0x00, 0x20, 0x20, 0x20, 0x20, 0x20,
0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
0x20, 0x20, 0x20, 0x20, 0x01, 0x08, 
0x82, 0x84, 0x8b, 0x96, 0x24, 0x30, 
0x48, 0x6c, 0x03, 0x01, 0x01,       
0x30, 0x18, 0x01, 0x00, 0x00, 0x0f, 0xac, 0x02, 0x02, 0x00,
0x00, 0x0f, 0xac, 0x04, 0x00, 0x0f, 0xac, 0x04, 
0x01, 0x00, 0x00, 0x0f, 0xac, 0x02, 0x00, 0x00
};

#include <ESP8266WebServer.h>

const uint8_t channels[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14}; // used Wi-Fi channels (available: 1-14)
const uint8_t channel1[] = {6, 7, 11, 12, 4};
const uint8_t channel2[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
const uint8_t channel3[] = {10, 11, 12, 13, 14};
const uint8_t channelr[] = { random(14) };
const bool wpa2 = true;
const bool appendSpaces = true; 
const char ssids[] PROGMEM = { 
    "There is a hacker here\n"
    "Click For Magic\n"
    "Being a Human bot\n"
    "C0me & Ask me 4 the passw0rd\n"
    "I Have Both Camera & WiFi\n"
    "AveΠgerZ – WiFi Wars\n"
    "Babez this 1 is just 4 U\n"
    "Being co_ol is all Mat_ters\n"
    "I Have Se_en U littering\n"
    "I Will Send U 2 the Jail\n"
    "Check Ur P0cket\n"
    "WiFi is Unavailable Only 4You!\n"
    "Mαrtin R0uter Kings WiFi\n"
    "Free WiFi For 1 Ni8 Stand\n"
    "Ur WiFi Iz Sle_eping Over\n"
    "Open the door\n"
    "Avengerz indestructible WiFi\n"
    "Penny Get Ur Own WiFi\n"
    "Dαnger Z0ne!\n"
    "Areα 51 zorks door\n"
    "De$perate HouseWiFi\n"
    "Call Me Anytimε\n"
    "1o1 Problems But Wi-Fi ain’t 1\n"
    "The Net $tarring $andra Bullock\n"
    "Save Wi-Fi save lives\n"
    "Skynet Global Defense CCTV\n"
    "DefaultmeLANs\n"
    "Stop polluting the environment.\n"
    "Wi-Fight\n"
    "Sunday,sunday,sunday\n"
    "One stop shop for your internet\n"
    "Why Not You Get Your WiFi?\n"
    "Something is at your face\n"
    "I em Under Ur Bed kid\n"
    "Pr0tected CEX\n"
    "Ermahgerd, WiFi!\n"
    "Stop staring me up\n"
    "Save water drink beer!\n"
    "Swedish Mafia Safehouse\n"
    "4 Guys 1 Router\n"
    "Smart Wi-Fi for smarttes\n"
    "Connect 4 fast Identity Theft\n"
    "Behind You\n"
    "Searching For the Network…\n"
    "Unlimited Downloading…\n"
    "Go-Go Bagget Internet\n"
    "We Don’t Load\n"
    "Wi-fi is Coming\n"
    "The Ma$ter of Whisperers\n"
    "The Brotherho_od Without LAN Er_rors\n"
    "The Ping Beyond the Wall\n"
    "The WiFi that was not promised\n"
    "The Dorni$hman’s Wi-Fi\n"
    "The Mαd Pings\n"
    "PingSlayer 2.1\n"
    "Vale of Arryn\n"
    "W!ght-Fi\n"
    "House of Black & Wi-Fi\n"
    "Ping of the West\n"
    "Brotherhood of LAN Errors\n"
    "PingsLαnding\n"
    "Browsing $trong\n"
    "The Night’s PiΠg\n"
    "Khaleesi Konnections\n"
    "Dothraki $ea\n"
    "Hot Wi\n"
    "The Cr0wnlands\n"
    "$tark 0f WinterFell\n"
    "Wi-Fi has no name\n"
    "Wi-Fire and Blood!\n"
    "House LANnister bloods\n"
    "John_Snow_Knows_Something\n"
    "Ping in the North!\n"
    "Westeros WinterNET fell\n"
    "The Ping Slayer 2.0\n"
    "Oathkeeper\n"
    "Live in the moment\n"
    "Put your mask back on\n"
    "LANnisters always pay their debts\n"
    "A Lan_nister Always Pays For Net\n"
    "WIFIght the inevitable\n"
    "The Dornishman’s WiFi\n"
    "Martell of $un$pear\n"
    "U Know Nothing LAN $now!\n"
    "Master of Wi-Fi\n"
    "Azor Wah Fai\n"
    "Fortress-of-Solitude:panic-room\n"
    "H0gsmeade\n"
    "Acci0WiFi\n"
    "R00m 0f Requirement\n"
    "Enemie$ 0f the He!r\n"
    "Flo_o Network\n"
    "Expelliarmus Ur Internet\n"
    "Blαck Link\n"
    "N0 Mugglez\n"
    "RavenClaw Com_mon R00m WiFi\n"
    "The Re$tricted $ection\n"
    "Fidelu$\n"
    "Hufflepuff Common Room\n"
    "Accio WiFi\n"
    "WIREgardium LE$$iosa\n"
    "The Secret of Dark\n"
    "Black iZ Cray\n"
    "Let The Wand Cho_ose It’s Wizard\n"
    "Hogwarts Scho_ol of Fre_e WiFi\n"
    "The Le@ky Cauldr0n\n"
    "Magic $ecure Muggle WiFi\n"
    "Magic $ecure\n"
    "Wizard AlwayZ\n"
    "Accio Internet!\n"
    "Dumbled0000re\n"
    "HA!L THE DARK L0RD\n"
    "H0rcrux\n"
    "The Guild of Krux\n"
    "Wi-Fideliu$\n"
    "The Elder Wi_Fi\n"
    "Albert0 WiFi\n"
    "Wh0mPing WiFi\n"
    "A Wizard Wireles_s\n"
    "Wizard Wireles_s\n"
    "Restricted Section 51\n"
    "Connect 2 Patronum\n"
    "Hogwarts School of WiFi & Wizardry\n"
    "Al0h0Mora\n"
    "Great Hall WiFi\n"
    "Hogwarts Expres_s\n"
    "I_Must_N0t_Tell_Lies\n"
    "Hermi0ne Grinder\n"
    "L0rd V0ldeModem\n"
    "Aloh0mora Crack the Pas_sword\n"
    "Ph0enix WiFi\n"
    "Gryffindor Common Room WiFi\n"
    "$lytherin Com_mon Ro_om WiFi\n"
    "Go Away Ur Muggle\n"
    "Chamber of $ecrets\n"
    "Abraa Kaa Daabra\n"
    "Hogwarts WiFi\n"
    "Patronus Charm\n"
    "I’m Batman\n"
    "Area 51 WACO\n"
    "2 Girls 1 cup Boy\n"
    "I am so Hungry\n"
    "Signal is Weak-as-hell\n"
    "My Password is Pass\n"
    "God is here\n"
    "Connect To Die\n"
    "UR My GF\n"
    "Internet is very painful Slow\n"
    "We are Virus\n"
    "hahaha! you have No internet\n"
    "This is Not For Everyone\n"
    "Only For Girls\n"
    "Do Not Disturb F!ing\n"
    "Unique WiFi Names\n"
    "FBI Surveillance Van #119871\n"
    "NSA Surveillance #392AF8\n"
    "NSA Surveillance #844AA7\n"
    "NSA Surveillance #732AB6\n"
    "NSA Surveillance #833AC3\n"
    "DEA Surveillance #4188A87\n"
    "DEA Surveillance Van #2288C8\n"
    "FBI Surveillance #117766\n"
    "I’m Watching You Now\n"
    "Skynet GDN\n"
    "Global Defense Skynet\n"
    "Let Me Out Of Your pron Router\n"
    "Undercover Police Car #751\n"
    "Police Car #586\n"
    "Undercover FBI Car #434\n"
    "I’m Cheating on my WiFi\n"
    "InterTubes\n"
    "Because a LANnister Never Forgets\n"
    "Never forget, expect us!\n"
    "Occam’s Router box\n"
    "New England Clam Router\n"
    "Please Click For Identity Theft\n"
    "Clinternet zone\n"
    "Anonymous WiFi\n"
    "I Can Haz the Wireless?\n"
    "IP Freely LANs\n"
    "Internet Access\n"
    "NSA Net Secure\n"
    "Really Untrusted Network\n"
    "Untrusted WiFi\n"
    "Virus Detected! Do Not Join\n"
    "Malware Repository\n"
    "Maleware thunb drive\n"
    "WiFi Ducky\n"
    "Access Denied fool\n"
    "Uncle_Touchys_Game_Room\n"
    "IP-UP-LAN-IP-I\n"
    "YerBabyIsReallyFat\n"
    "John Wilkes WiFi\n"
    "Slow WiFi\n"
    "Iron Ducky here\n"
    "1Optimus Prime\n"
    "WamBamThankYouLan\n"
    "TellMyWiFiLoveHer\n"
  "Under Your Ded now\n"
  "I'am under the bar\n"
  "LOLZ/b/Random\n"
  "http://www.4chan.org\n"
  "8chan.org\n"
  "Mom Use This One\n"
  "InyourWheelHouse\n"
  "Bro, get-off-my pornbox\n"
  "Brah! use this one\n"
  "Abraham Linksys\n"
  "No More fake news!\n"
  "Welcome back MAGA\n"
  "Biden is god\n"
  "Benjamin FrankLAN\n"
  "Biden is evil \n"
  "Inferno Peanuts\n"
  "HOT&SPICY\n"
  "999SATAN666\n"
  "I am in your car\n"
  "Zardos, The Spam is good!\n"
  "HerBack Door pass\n"
  "Bitches Gone WiFi\n"
  "bro in your van\n"
  "The Gang gets WiFi\n"
  "CapsLock is on\n"
  "In your car\n"
  "POP the trunk\n"
  "Dump Trump\n"
  "Hack loading ..\n"
  "infowarsstoree.com\n"
  "Rocket WiFi 10Kbps\n"
  "Pocket Rocket \n"
  "Glory Hole WiFi\n"
  "Pool-Pocket pro\n"
  "#BlackDicksMatter\n"
  "Martin Router King\n"
  "John Wilkes Bluetooth\n"
  "Pretty Fly for a Wi-Fi\n"
  "Bill Wi the Science Fi\n"
  "I Believe Wi Can Fi\n"
  "Tell My Wi-Fi Love Her\n"
  "No More Mister Wi-Fi\n"
  "LAN Solo\n"
  "I AM LAN LOCKED\n"
  "The LAN Before Time\n"
  "Silence of the LANs\n"
  "House LANister\n"
  "Winternet Is Coming\n"
  "Ping’s Landing\n"
  "The Ping in the North\n"
  "This LAN Is My LAN\n"
  "Get Off My LAN\n"
  "SUPERvised LAN\n"
  "The Promised LAN\n"
  "The LAN Down Under\n"
  "FBI Surveillance Van 4\n"
  "Area 51 Test Site\n"
  "Drive-By Wi-Fi\n"
  "Planet Express\n"
  "Wu Tang LAN\n"
  "Darude LANstorm\n"
  "Never Gonna Give You Up\n"
  "Hide Yo Kids, Hide Yo Wi-Fi\n"
  "Loading…\n"
  "Searching…\n"
  "VIRUS.EXE\n"
  "1% 2% 3%\n"
  "Virus-Infected Wi-Fi\n"
  "Starbucks Wi-Fi\n"
  "Text ###-#### for Password\n"
  "Yell ____ for Password\n"
  "The Password Is 1234\n"
  "Free Public Wi-Fi\n"
  "No Free Wi-Fi Here\n"
  "Get Your Own Damn Wi-Fi\n"
  "It Hurts When IP\n"
  "Dora the Internet Explorer\n"
  "404 Wi-Fi Unavailable\n"
  "Porque-Fi\n"
  "Titanic Syncing\n"
  "Test Wi-Fi Please Ignore\n"
  "Drop It Like It’s Hotspot\n"
  "Life in the Fast LAN\n"
  "The Creep Next Door\n"
  "Ye Olde Internet\n"
  "Mother lan FUCKERS \n"
  "RockHard LAN\n"
  "cocksuckers\n"
  "MikeShesCheatingOnYou\n"
  "1Kpbs Speed\n"
  "LAN Down UNDER baby\n"
  "TwoWhiteBitches\n"
  "Infected WiFi\n"
  "Titanic Synching\n"
  "Click-Here-for-free-virus\n"
  "disabled SSID\n"
  "I Pronounce you Man and WiFI\n"
  "I did your Wifi Last Night\n"
  "Happy Wifi Happy Life!\n"
  "$10 per hour\n"
  "PornTorrent max\n"
  "PornBox\n"
  "Beer and bitches bro!\n"
  "AOL Dialup\n"
  "TheodoreFrankLAN\n"
  "Vladimir Routin\n"
  "Your WiFi has COVID-19\n"
  "John-Claude-WAN-Damme\n"
  "FreeSpeechZone\n"
  "I am Iron LAN\n"
  "Spider LAN, Spider LAN\n"
  "DoLAN Trump\n"
  "404 Wi-Fi Unavailable\n"
  "Virus Distribution Center\n"
  "your Dog shits in my yard\n"
  "Free WiFi, Just Kidding\n"
  "your mesic is annoying\n"
  "infowars.com\n"
  "prisonplanet.tv\n"
  "poor boy signal\n"
  "No Facebook, Happy Life\n"
  "Hey! We can hear U while S*x\n"
  "Making Wi-Fi Great Once Again\n"
  "vladimir computin\n"
  "WiMCA\n"
  "1000100001111011111101111\n"
  "1110111011101110111011101\n"
  "I0I 101\n"
  "www.Pornhub.com\n"
  "Free Dick pics?\n"
  "1000mbps add for me losers\n"
  "GetOffMyLAN\n"
  "I can hear you Farting!\n"
  "Immigration Authorities\n"
  "NiceTryNeighbor\n"
  "TheBotZareWatchingYou\n"
  "www.4chan.org\n"
  "Pay berfore you Digi-dump\n"
  "Hacking your WiFi !\n"
  "CCTV Bathroom mens\n"
  "CCTV Bathroom hidden\n"
  "FBI Van A0938EG\n"
  "MAGA Trump4MoreYRS\n"
  "FU-BAR-WiFi\n"
  "Gitchers WiFi\n"
  "Golden-WiFi\n"
  "Loser-retard-wifi\n"
  "LAN of the lost\n"
  "LanHansel1\n"
  "your-moms-backdoor\n"
    "Promised WiFi Power Range\n"
    "We_don’t_have_any_WiFi\n"
    "Hαppiness is When Ü know them.\n"
    "Lazy sloth Internet!\n"
    "Yell [Pene$] 4 Password\n"
    "No More WiFi man\n"
    "Everytime em Buffering\n"
    "Password Free\n"
    "N0 Pant$ N0 Problem$\n"
    "GirlZz G⍥NE Wireless ?\n"
    "Hilαry Clint£rnet\n"
    "Mine Number 09000000\n"
    "∞ Speed\n"
    "Bill Wi, the $cience Fi\n"
    "Traffic Sucker ?\n"
    "WinTernet I$ Here ❄️\n"
    "Loading Failed\n"
    "BobZ Un$ecured Hou$e of WiFi\n"
    "Con_nect 2 Die\n"
    "Troy & Abed in the Modem$\n"
    "Fear of dogs\n"
    "A co_ol head and glowing Wi-Fi\n"
    "Only Your WiFi\n"
    "It Really βurns When IP ?\n"
    "Get off My LANss!\n"
    "Restricted Section\n"
    "Property of mine \n"
    "sexy girlfriend only ?\n"
    "Go_od WiFi\n"
    "Fre_e WiFi Available Here\n"
    "The LAN βefore Time\n"
    "Nacho lebray WiFi\n"
    "Huh next time secure your router\n"
    "Fre_e WiFi\n"
    "Fly for a WiFi\n"
    "Get Ur OwNET Bruh\n"
    "Only for my sexy wife\n"
    "M00CHES WIFI\n"
    "Dora The Internet Explorer\n"
    "Stop Being A M00ch\n"
    "Good Wi-Fi c00l extra than cold water\n"
    "Bens WiFi\n"
    "Obtaining the IP Address\n"
    "424: Wi-Fi Unavailable\n"
    "Choose a Network already\n"
    "Waiting for the connection\n"
    "Unknown Device\n"
    "Connecting to yahoo…\n"
    "Router Failure\n"
    "IP Connection Refused\n"
    "Unavailable Now\n"
    "Prohibited suckers\n"
    "The awesome reset\n"
    "No Networks Available\n"
    "Incorrect password entered\n"
    "Error: Please Contact Your ISP\n"
    "Direct Satellite Home Transfer\n"
    "TCP/IP ERROR 42\n"
    "Setting upppp\n"
    "Testing stupid idea\n"
    "Signing Off\n"
    "Server Error falt\n"
    "Connection Out\n"
    "Qwest timed out\n"
    "Searching…...\n"
    "Loading…...\n"
    "Need Antivirus Software\n"
    "Use at your own risk\n"
    "Yo Access Denied!\n"
    "Under Maintenance\n"
    "You have been blocked\n"
    "Not In Use\n"
    "internet connectivity\n"
    "Network_Error_410\n"
    "168.1.1\n"
    "192.168.1.4\n"
    "Connection Lost\n"
    "Network is down\n"
    "Airplane Mode Active\n"
    "DHCP_Failure_Error\n"
    "Device Battery Low\n"
    "Shutting Down shit\n"
    "Service Interrupted\n"
    "Noooooo.Disconnected\n"
    "Dial-up 56k Internet\n"
    "RFID Tag Reader\n"
    "dns_unresolved_hostname\n"
    "Limit Exceeded retard\n"
    "Signals not found\n"
    "Gateway 666 Error\n"
    "Searching for network dork!\n"
    "Android Bluetooth\n"
    "Router Malfunction\n"
    "Device Damaged\n"
    "Bean falls down\n"
    "My Wi-FI Love Her\n"
    "Can I be Frank LAN\n"
    "Area 51 B Test Site\n"
    "The Creeps pron LAN\n"
    "Get Your Own Wi-Fi\n"
    "wireless gang bang 2\n"
    "NOTFree Public Wi-Fi\n"
    "Yell help for Password\n"
    "Starbucks Wi-Fi front\n"
    "The Legion of Doom\n"
    "That’s HotSpot\n"
    "Porque-Fi\n"
    "LANdo\n"
    "Yummy!!\n"
    "The Notorious R.P.G.\n"
    "No, Mo-More Mister Wi-Fi\n"
    "The Password Is 1234\n"
    "Why do you get WiFi?\n"
    "LANister?\n"
    "The Deathstar WiFi\n"
    "The Banana Stand\n"
    "ye-old LANs\n"
    "Wi-Fi Please Ignore\n"
    "Never Gonna Give You Up!!\n"
    "No Free Wi-Fi \n"
    "Text 406-###-#### for Password\n"
    "404 Sllooww Wi-Fi Unavailable\n"
    "It Hurts When IP LAN\n"
    "Life in the Fast LAN 2.0\n"
    "GeoLANstorm\n"
    "FBI Surveillance Van 34E4AS\n"
    "The Ping in the North\n"
    "Wi-fight the inevitable\n"
    "wireless router.\n"
    "WORLD tour\n"
    "Ping’s LANding\n"
    "EXE\n"
    "Dora the Internet Explorer 2\n"
    "Virus-Infected Wi-Fi 2\n"
    "This LAN Is My LAN 2\n"
    "Total Hear\n"
    "Ye Olde Internet 2\n"
    "no, this is not free either\n"
    "Silence of the LANsss\n"
    "I Believe Wi Can Fi\n"
    "1UP Mushroom\n"
    "2Girls1Dorm\n"
    "And I Thought They Smelled Bad On The Outside\n"
    "Andromeda\n"
    "At least I get laid loser\n"
    "BeyHive\n"
    "Bill Wi the Science Fi 2\n"
    "Bill! Bill! Bill! Bill! Bill!\n"
    "Bluth Company\n"
    "Bowser’s Castle\n"
    "Close your blinds, dude!\n"
    "Don’t look under the bed\n"
    "Drop It Like Its HotSpot\n"
    "Dunder-Mifflin\n"
    "DungeonMaster\n"
    "Ermahgerd Wer Fer\n"
    "Fortress of Solitude\n"
    "Getyourownnetbro\n"
    "GucciLouisPrada\n"
    "Hall of Justice\n"
    "Hide Yo, Hide Yo Wifi\n"
    "I Got 99 Problems But WiFi Ain’t 1\n"
    "I killed your cat\n"
    "I Spy With WiFi\n"
    "I watch you guys all night\n"
    "Icanseeyourightnow\n"
    "the Rat hole \n"
    "Dirty carbon unit\n"
    "ImUnderYourBed\n"
    "InternetCosts$\n"
    "I’mRickJamesB!tch\n"
    "I’ve Seen You Naked\n"
    "Jabba the Hut’s Lair\n"
    "JumpmanJumpmanJumpman\n"
    "Keep It On The Download\n"
    "Krypton\n"
    "LanDownUnderBro!\n"
    "Lansanity\n"
    "gather for Jesus\n"
    "MakeAmericaGreatWithFreeWiFiAgain\n"
    "Millennium Falcon\n"
    "No Wifi For You geeky!\n"
    "NoFreeInternetHereKeepLooking\n"
    "NotMyPresident\n"
    "Notthedroidsyouarelookingfor\n"
    "One Does Not Simply Use WiFi\n"
    "One Small Step For Man, One Giant Leap For WiFi\n"
    "Parks and Recreation WiFi\n"
    "The shitters WiFi\n"
    "KABOOOM!\n"
    "FUCK-IT!\n"
    "Hit-me-in-the-bum-bum\n"
    "Iron Ducky\n"
    "IronDucky HanSel 2\n"
    "Iron Ducky 54\n"
    "InCEL 4 life\n"
    "Iron Ducky 33\n"
    "I am IRON LAN 2\n"
    "Iron Ducky 4\n"
    "I computer lab\n"
    "Iron Ducky 34\n"
    "Space Invader\n"
    "Iron Ducky 45\n"
    "8bit duck hunter\n"
    "Iron Ducky 49\n"
    "WhitehotBlackText\n"
    "Iron Ducky 22\n"
    "Its HAM RADIO\n"
    "Iron Ducky 11\n"
    "Its the cops run!\n"
    "Iron Ducky 23\n"
    "QC to CQ \n"
    "Iron Ducky 2\n"
    "Liberal protest\n"
    "IronDucky HanSel\n"
    "Iron Ducky 1\n"
    "666 SATAN 999\n"
    "Iron Ducky 3\n" 
};
const char* ssid = "IronDucky(HanSel)";
const char* password = "4Antinetmass";
const int GREEN = D6;
const int RED = D8;
const int BLUE = D7;
const int LIGHT_SENSOR = A0;
const int  BUTTON = 4;
int val = 0; 
int analogValue = analogRead(LIGHT_SENSOR);
 
/* Setup local IP address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);

void setup() {
  pinMode(RED, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  Serial.begin(115200); 
  Serial.println(" "); td();
  Serial.print("Testing RGB color led on witty cloud...");
  analogWrite(RED, 255); td(); analogWrite(RED, 0); td();
  analogWrite(BLUE, 255); td(); analogWrite(BLUE, 0); td();
  analogWrite(GREEN, 255); td(); analogWrite(GREEN, 0); td();
  Serial.println("..Done.");
  Serial.println("Laoding server paths");
  WiFi.softAP(ssid, password);  
  WiFi.softAPConfig(local_ip, gateway, subnet); td();
  server.on("/", handle_OnConnect);
  server.on("/spam", handle_spam);
  server.on("/stat", handle_stat);
  server.on("/help", handle_help);   
  server.on("/mode1", handle_mode1);
  server.on("/mode2", handle_mode2);
  server.on("/mode3", handle_mode3);
  server.on("/mode4", handle_mode4);
  server.on("/light", handle_light); 
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println("192.168.1.1:80");
  Serial.println("HTTP server started:");
  led_ok();
}


void loop() {
 server.handleClient();
  currentTime = millis();
  val = digitalRead(BUTTON);
  if (val == HIGH) {
  server.handleClient();  
  } else {
  Serial.println("Button pressed panic mode active");
  handle_mode1();
  }
  // DO led object delay 7799 millis
  if (currentTime - attackTime > 7799) {
    attackTime = currentTime;
    led_TX();
    server.handleClient();
  }
 
}

void handle_light() {
  led_ok();
  led_RX();
  int analogValue = analogRead(A0);
  if(analogValue < 10) {
   server.send(200, "text/plain",  "Light level dark"); 
  } else if (analogValue < 200) {
    server.send(200, "text/plain", "Light level dim");
  } else if (analogValue < 500) {
    server.send(200, "text/plain", "Light level soft light");
  } else if (analogValue < 800) {
    server.send(200, "text/plain", "Light level is very bright");
  } else {
    server.send(200, "text/plain", "Default send");
  }
  Serial.print("Light value is = ");
  Serial.println(analogValue);
  led_TX();
}

void handle_OnConnect() 
{
  led_rxtx();
  Serial.println("Server status: / 200 text/html send.");
  led_ok();
  server.send(200, "text/html", SendHTMLmain());
}

void handle_mode1() {
  led_TX();
  server.send(200, "text/html", SendHTMLmode1());
  led_ok();
  Serial.println("Server status: /mode1 200 text/html send.");
  Serial.println("Running sub functions....");
  led_working();
  handle_spam();
}

void handle_mode2() {
  led_TX();
  server.send(200, "text/html", SendHTMLmode2());
  led_ok();
  Serial.println("Server status: /mode2 200 text/html send.");
  Serial.println("Running sub functions....");
  led_working();
  handle_attack2();
}

void handle_mode3() {
  led_TX();
  server.send(200, "text/html", SendHTMLmode3());
  led_ok();
  Serial.println("Server status: /mode2 200 text/html send.");
  Serial.println("Running sub functions....");
  led_working();
  handle_attack3();
}

void handle_mode4() {
  led_TX();
  server.send(200, "text/html", SendHTMLmode4());
  led_ok();
  Serial.println("Server status: /mode2 200 text/html send.");
  Serial.println("Running sub functions....");
  led_working();
  handle_attack4();
}

void handle_help() {
  led_TX(); 
  Serial.println("Server status: /help 200 text/html send.");
  server.send(200, "text/html", SendHTMLmain());
  led_ok();
}


void handle_attack4() 
{
  Serial.println("Server status: /mode3 200 text/html send.");
  server.send(101, "text/html"); 
  led_ok();
  server.send(200, "text/html", SendHTMLspam());
  Serial.println("Server status: /mode3 101 text/html send.");
  Serial.println("Server status: DOWN / Attack mode on / ");
  setup_attack4(); 
   for (int x=555; x>=1; x--) {    // temp variables
    led_TX();
    int i = 0;
    int j = 0;
    int ssidNum = 1;
    char tmp;
    int ssidsLen = strlen_P(ssids);
    bool sent = false;

    // Go to next channel
    nextChannel();

    while (i < ssidsLen) {
      // Get the next SSID
      j = 0;
      do {
        tmp = pgm_read_byte(ssids + i + j);
        j++;
      } while (tmp != '\n' && j <= 32 && i + j < ssidsLen);

      uint8_t ssidLen = j - 1;

      // set MAC address
      macAddr[5] = ssidNum;
      ssidNum++;

      // write MAC address into beacon frame
      memcpy(&beaconPacket[10], macAddr, 6);
      memcpy(&beaconPacket[16], macAddr, 6);

      // reset SSID
      memcpy(&beaconPacket[38], emptySSID, 32);

      // write new SSID into beacon frame
      memcpy_P(&beaconPacket[38], &ssids[i], ssidLen);

      // set channel for beacon frame
      beaconPacket[82] = wifi_channel;

      // send packet
      if (appendSpaces) {
        for (int k = 0; k < 3; k++) {
          packetCounter += wifi_send_pkt_freedom(beaconPacket, packetSize, 0) == 0;
          delay(1);
        }
      }

      // remove spaces
      else {

        uint16_t tmpPacketSize = (packetSize - 32) + ssidLen; // calc size
        uint8_t* tmpPacket = new uint8_t[tmpPacketSize]; // create packet buffer
        memcpy(&tmpPacket[0], &beaconPacket[0], 38 + ssidLen); // copy first half of packet into buffer
        tmpPacket[37] = ssidLen; // update SSID length byte
        memcpy(&tmpPacket[38 + ssidLen], &beaconPacket[70], wpa2 ? 39 : 13); // copy second half of packet into buffer

        // send packet
        for (int k = 0; k < 3; k++) {
          packetCounter += wifi_send_pkt_freedom(tmpPacket, tmpPacketSize, 0) == 0;
          delay(1);
        }

        delete tmpPacket; // free memory of allocated buffer
      }

      i += j;
    }
    led_RX();
   }
}

void handle_attack3() 
{
  Serial.println("Server status: /mode3 200 text/html send.");
  server.send(101, "text/html"); 
  led_ok();
  server.send(200, "text/html", SendHTMLspam());
  Serial.println("Server status: /mode3 101 text/html send.");
  Serial.println("Server status: DOWN / Attack mode on / ");
  setup_attack3(); 
   for (int x=555; x>=1; x--) {    // temp variables
    led_TX();
    int i = 0;
    int j = 0;
    int ssidNum = 1;
    char tmp;
    int ssidsLen = strlen_P(ssids);
    bool sent = false;

    // Go to next channel
    nextChannel();

    while (i < ssidsLen) {
      // Get the next SSID
      j = 0;
      do {
        tmp = pgm_read_byte(ssids + i + j);
        j++;
      } while (tmp != '\n' && j <= 32 && i + j < ssidsLen);

      uint8_t ssidLen = j - 1;

      // set MAC address
      macAddr[5] = ssidNum;
      ssidNum++;

      // write MAC address into beacon frame
      memcpy(&beaconPacket[10], macAddr, 6);
      memcpy(&beaconPacket[16], macAddr, 6);

      // reset SSID
      memcpy(&beaconPacket[38], emptySSID, 32);

      // write new SSID into beacon frame
      memcpy_P(&beaconPacket[38], &ssids[i], ssidLen);

      // set channel for beacon frame
      beaconPacket[82] = wifi_channel;

      // send packet
      if (appendSpaces) {
        for (int k = 0; k < 3; k++) {
          packetCounter += wifi_send_pkt_freedom(beaconPacket, packetSize, 0) == 0;
          delay(1);
        }
      }

      // remove spaces
      else {

        uint16_t tmpPacketSize = (packetSize - 32) + ssidLen; // calc size
        uint8_t* tmpPacket = new uint8_t[tmpPacketSize]; // create packet buffer
        memcpy(&tmpPacket[0], &beaconPacket[0], 38 + ssidLen); // copy first half of packet into buffer
        tmpPacket[37] = ssidLen; // update SSID length byte
        memcpy(&tmpPacket[38 + ssidLen], &beaconPacket[70], wpa2 ? 39 : 13); // copy second half of packet into buffer

        // send packet
        for (int k = 0; k < 3; k++) {
          packetCounter += wifi_send_pkt_freedom(tmpPacket, tmpPacketSize, 0) == 0;
          delay(1);
        }

        delete tmpPacket; // free memory of allocated buffer
      }

      i += j;
    }
    led_RX();
   }
}

void handle_attack2() 
{
  Serial.println("Server status: /mode2 200 text/html send.");
  server.send(101, "text/html"); 
  led_ok();
  server.send(200, "text/html", SendHTMLspam());
  Serial.println("Server status: /mode2 101 text/html send.");
  Serial.println("Server status: DOWN / Attack mode on / ");
  setup_attack2(); 
   for (int x=555; x>=1; x--) {    // temp variables
    led_TX();
    int i = 0;
    int j = 0;
    int ssidNum = 1;
    char tmp;
    int ssidsLen = strlen_P(ssids);
    bool sent = false;

    // Go to next channel
    nextChannel();

    while (i < ssidsLen) {
      // Get the next SSID
      j = 0;
      do {
        tmp = pgm_read_byte(ssids + i + j);
        j++;
      } while (tmp != '\n' && j <= 32 && i + j < ssidsLen);

      uint8_t ssidLen = j - 1;

      // set MAC address
      macAddr[5] = ssidNum;
      ssidNum++;

      // write MAC address into beacon frame
      memcpy(&beaconPacket[10], macAddr, 6);
      memcpy(&beaconPacket[16], macAddr, 6);

      // reset SSID
      memcpy(&beaconPacket[38], emptySSID, 32);

      // write new SSID into beacon frame
      memcpy_P(&beaconPacket[38], &ssids[i], ssidLen);

      // set channel for beacon frame
      beaconPacket[82] = wifi_channel;

      // send packet
      if (appendSpaces) {
        for (int k = 0; k < 3; k++) {
          packetCounter += wifi_send_pkt_freedom(beaconPacket, packetSize, 0) == 0;
          delay(1);
        }
      }

      // remove spaces
      else {

        uint16_t tmpPacketSize = (packetSize - 32) + ssidLen; // calc size
        uint8_t* tmpPacket = new uint8_t[tmpPacketSize]; // create packet buffer
        memcpy(&tmpPacket[0], &beaconPacket[0], 38 + ssidLen); // copy first half of packet into buffer
        tmpPacket[37] = ssidLen; // update SSID length byte
        memcpy(&tmpPacket[38 + ssidLen], &beaconPacket[70], wpa2 ? 39 : 13); // copy second half of packet into buffer

        // send packet
        for (int k = 0; k < 3; k++) {
          packetCounter += wifi_send_pkt_freedom(tmpPacket, tmpPacketSize, 0) == 0;
          delay(1);
        }

        delete tmpPacket; // free memory of allocated buffer
      }

      i += j;
    }
    led_RX();
   }
}

void handle_spam() 
{
  Serial.println("Server status: /spam 200 text/html send.");
  server.send(101, "text/html"); 
  led_ok();
  server.send(200, "text/html", SendHTMLspam());
  Serial.println("Server status: /spam 101 text/html send.");
  Serial.println("Server status: DOWN / Attack mode on / ");
  setup_attack(); 
   for (int x=555; x>=1; x--) {    // temp variables
    led_TX();
    int i = 0;
    int j = 0;
    int ssidNum = 1;
    char tmp;
    int ssidsLen = strlen_P(ssids);
    bool sent = false;

    // Go to next channel
    nextChannel();

    while (i < ssidsLen) {
      // Get the next SSID
      j = 0;
      do {
        tmp = pgm_read_byte(ssids + i + j);
        j++;
      } while (tmp != '\n' && j <= 32 && i + j < ssidsLen);

      uint8_t ssidLen = j - 1;

      // set MAC address
      macAddr[5] = ssidNum;
      ssidNum++;

      // write MAC address into beacon frame
      memcpy(&beaconPacket[10], macAddr, 6);
      memcpy(&beaconPacket[16], macAddr, 6);

      // reset SSID
      memcpy(&beaconPacket[38], emptySSID, 32);

      // write new SSID into beacon frame
      memcpy_P(&beaconPacket[38], &ssids[i], ssidLen);

      // set channel for beacon frame
      beaconPacket[82] = wifi_channel;

      // send packet
      if (appendSpaces) {
        for (int k = 0; k < 3; k++) {
          packetCounter += wifi_send_pkt_freedom(beaconPacket, packetSize, 0) == 0;
          delay(1);
        }
      }

      // remove spaces
      else {

        uint16_t tmpPacketSize = (packetSize - 32) + ssidLen; // calc size
        uint8_t* tmpPacket = new uint8_t[tmpPacketSize]; // create packet buffer
        memcpy(&tmpPacket[0], &beaconPacket[0], 38 + ssidLen); // copy first half of packet into buffer
        tmpPacket[37] = ssidLen; // update SSID length byte
        memcpy(&tmpPacket[38 + ssidLen], &beaconPacket[70], wpa2 ? 39 : 13); // copy second half of packet into buffer

        // send packet
        for (int k = 0; k < 3; k++) {
          packetCounter += wifi_send_pkt_freedom(tmpPacket, tmpPacketSize, 0) == 0;
          delay(1);
        }

        delete tmpPacket; // free memory of allocated buffer
      }

      i += j;
    }
    led_RX();
   }
}

void handle_NotFound(){
  Serial.println("Server status: 404 file not found 400 text/html sned.");
  server.send(404, "text/html", Send404HTML());
  led_fail();
}


void handle_stat() {
  Serial.println("Server status: 200 text/html /stat sent.");
  server.send(200, "text/html", SendHTMLstat());
  led_ok();
}

void led_fail() {
  for (int i=11; i>=1; i--) {
    analogWrite(RED, 255); td(); analogWrite(RED, 0); td(); 
  }
}

void led_ok() {
  for (int i=2; i>=1; i--) {
    analogWrite(GREEN, 255); td(); analogWrite(GREEN, 0); td(); 
  }
}

void led_working() {
  for (int i=3; i>=1; i--) {
    analogWrite(RED, 255); td(); 
    analogWrite(BLUE, 255); td();
    analogWrite(GREEN, 255); td();
    analogWrite(GREEN, 0); td();
    analogWrite(BLUE, 0); td();
    analogWrite(RED, 0); td();
  }
}

void led_TX() {
     analogWrite(BLUE, 255); td();
     analogWrite(BLUE, 0); td(); 
}

void led_RX() {
     analogWrite(RED, 155); td();
     analogWrite(RED, 0); td(); 
}

void led_working2() {
    analogWrite(RED, 255); td(); 
    analogWrite(BLUE, 255); td();
    analogWrite(GREEN, 255); td();
    analogWrite(GREEN, 0); td();
    analogWrite(BLUE, 0); td();
    analogWrite(RED, 0); td();  
}

void led_rxtx() {
  for (int i=2; i>=1; i--) {
    analogWrite(RED, 255); td(); analogWrite(RED, 0); td();
    analogWrite(GREEN, 255); td(); analogWrite(GREEN,0); td();
  }
}

void td() { delay(200); }

String SendHTMLmode2() {
String ptr = "<!DOCTYPE html><html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>Sweeping mode Active (Hansel remote Server droid 1.2Bata)</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px; background-color:black;} h1 {color: green; margin: 50px auto 30px; size:30px;} h2 {color: blue;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #1abc9c;}\n";
  ptr +=".button-on:active {background-color: #16a085;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="ul { color: white; } li { color: white; }";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body bgcolor='black' color='white'>\n";
  ptr +="<center><h1>SWEEPING MODE ACTIVE</h1></center>\n";
  ptr +="<p> Server hideing ESSID active! Please reset device end attack mode.<br>\n";
  ptr +="My select WiFi channels i am now sweeping [6, 7, 11, 12, 4].<br>\n";
  ptr +="All other LAN channels are safe for now. ";
  ptr +="<ul> \n";
  ptr +="<li>ESP8266 Web Server <font color='red'>Diabled</font></li> \n";
  ptr +="<li>Using Station(STA) Mode <font color='red'>Disabled</font></li>\n";
  ptr +="<li>WiFi OFF: (STATION_MODE) spoffing random MAC addresses <font color='red'>Active</font></li> \n";
  ptr +="</ul></p>";
  ptr +="</body>\n";
  ptr +="</html>\n";
  
  return ptr;
}

String SendHTMLmode3() {
String ptr = "<!DOCTYPE html><html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>Sweeping mode Active (Hansel remote Server droid 1.2Bata)</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px; background-color:black;} h1 {color: green; margin: 50px auto 30px; size:30px;} h2 {color: blue;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #1abc9c;}\n";
  ptr +=".button-on:active {background-color: #16a085;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="ul { color: white; } li { color: white; }";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body bgcolor='black' color='white'>\n";
  ptr +="<center><h1>SWEEPING MODE ACTIVE</h1></center>\n";
  ptr +="<p> Server hideing ESSID active! Please reset device end attack mode.<br>\n";
  ptr +="Sweeping channels 1 to 9 only with spam.\n";
  ptr +="<ul> \n";
  ptr +="<li>ESP8266 Web Server <font color='red'>Diabled</font></li> \n";
  ptr +="<li>Using Station(STA) Mode <font color='red'>Disabled</font></li>\n";
  ptr +="<li>WiFi OFF: (STATION_MODE) spoffing random MAC addresses <font color='red'>Active</font></li> \n";
  ptr +="</ul></p>";
  ptr +="</body>\n";
  ptr +="</html>\n";
  
  return ptr;
}

String SendHTMLmode4() {
String ptr = "<!DOCTYPE html><html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>Sweeping mode Active 10 - 14(Hansel remote Server droid 1.2Bata)</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px; background-color:black;} h1 {color: green; margin: 50px auto 30px; size:30px;} h2 {color: blue;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #1abc9c;}\n";
  ptr +=".button-on:active {background-color: #16a085;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="ul { color: white; } li { color: white; }";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body bgcolor='black' color='white'>\n";
  ptr +="<center><h1>SWEEPING MODE ACTIVE</h1></center>\n";
  ptr +="<p> Server hideing ESSID! Please reset device end attack mode.<br>\n";
  ptr +="<font color='red'>Sweeping mode channels 10 to 14 </font><br>\n"; 
  ptr +="<ul> \n";
  ptr +="<li>ESP8266 Web Server <font color='red'>Diabled</font></li> \n";
  ptr +="<li>Using Station(STA) Mode <font color='red'>Disabled</font></li>\n";
  ptr +="<li>WiFi OFF: (STATION_MODE) spoffing random MAC addresses <font color='red'>Active</font></li> \n";
  ptr +="</ul></p>";
  ptr +="</body>\n";
  ptr +="</html>\n";
  
return ptr;  
}

String SendHTMLmode1(){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>Panic mode Active (Hansel remote Server droid 1.2Bata)</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px; background-color:black;} h1 {color: green; margin: 50px auto 30px; size:30px;} h2 {color: blue;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #1abc9c;}\n";
  ptr +=".button-on:active {background-color: #16a085;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="ul { color: white; } li { color: white; }";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body bgcolor='black' color='white'>\n";
  ptr +="<center><h1>PANICROOM MODE ACTIVE</h1></center>\n";
  ptr += "<p> Server hideing ESSID push button active! Please reset device end attack mode.<br>\n";
  ptr +="<ul> \n";
  ptr +="<li>ESP8266 Web Server <font color='red'>Diabled</font></li> \n";
  ptr +="<li>Using Station(STA) Mode <font color='red'>Disabled</font></li>\n";
  ptr +="<li>WiFi OFF: (STATION_MODE) spoffing random MAC addresses <font color='red'>Active</font></li> \n";
  ptr +="</ul></p>";
  ptr +="</body>\n";
  ptr +="</html>\n";
  
  return ptr;
}

String SendHTMLstat(){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>Hansel remote Server droid 1.2Bata</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px; background-color:black;} h1 {color: red; margin: 50px auto 30px;} h2 {color: blue;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #1abc9c;}\n";
  ptr +=".button-on:active {background-color: #16a085;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="ul { color: white; } li { color: white; }";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body bgcolor='black' color='white'>\n";
  ptr +="<h1>Server information</h1> \n";
  ptr += "<p> ESSID: LanHansel <br>\n PASSWORD: 4Antinetmass <br>\n";
  ptr += "Local IP:192.168.1.1<br>\n Gateway:192.168.1.1 <br>\n Subnet:255.255.255.0 <br>\n";
  ptr +="<h2>Server setup</h2> \n";
  ptr +="<ul> \n";
  ptr +="<li>ESP8266 Web Server <font color='red'>ACTIVE</font></li> \n";
  ptr +="<li>Using Station(STA) Mode <font color='red'>ACTIVE</font></li>\n";
  ptr +="</ul>";
  ptr +="</body>\n";
  ptr +="</html>\n";
  
  return ptr;
}

String Send404HTML(){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>LED Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px; background:black; text-color:white;} h1 {color: #444444;margin: 50px auto 30px; font-size:50px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>404</h1>\n";
  ptr +="<h3>Not found on server..</h3>\n";
  ptr +="<p>Note: server is active if you see this message.</p>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";  
  return ptr;
}

String SendHTMLspam(){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>LED Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #1abc9c;}\n";
  ptr +=".button-on:active {background-color: #16a085;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>ESP8266 Web Server Setup/Resetting for attack</h1>\n";
  ptr +="<h3>Using Station(STA) Mode ... Shutting down server ...</h3>\n";
  ptr +="<br><p>ESSID spaming is now running reset button to end process.</p> \n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}


String SendHTMLmain() {
  String ptr = "<!DOCTYPE html><html>\n";
  ptr +="<head><title>ESP8266 device active</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px; background-color: black; font-color: white;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 480px;background-color: #1abc9c; border: 1px; color: white; padding: 3px 3px;text-decoration: none; font-size: 25px; margin: 0px auto 35px; cursor: pointer;border-radius: 4px;}\n";
  ptr +="font { color: white; display: block;width: 480px;} \n";
  ptr +="ol {display: block;width: 480px; color: white; }  \n"; 
  ptr +="h2 {display: block;width: 480px; color: green; } \n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style></head>\n";
  ptr +="<body>\n";
  ptr +="<br /> <h1>Directory assistance</h1>\n";
  ptr +="<form><ol>  \n";
  ptr +="<li><button><a href='http://192.168.1.1/stat'> http://192.168.1.1/stat</a></button></li>\n";
  ptr +="<li><button><a href='http://192.168.1.1/spam'> http://192.168.1.1/spam</a></button></li>\n";
  ptr +="<li><button><a href='http://192.168.1.1/help'>http://192.168.1.1/help</a> </button></li>\n";
  ptr +="<li><button><a href='http://192.168.1.1/mode1'> http://192.168.1.1/mode1</a></button></li>\n";
  ptr +="<li><button><a href='http://192.168.1.1/mode2'> http://192.168.1.1/mode2</a></button></li>\n";
  ptr +="<li><button><a href='http://192.168.1.1/mode3'> http://192.168.1.1/mode3</a></button></li>\n";
  ptr +="<li><button><a href='http://192.168.1.1/mode4'> http://192.168.1.1/mode4</a></button></li>\n";  
  ptr +="</ol></form>\n";
  ptr +="<h2>About directorys root / </h2> <br>\n";
  ptr +="<font color='white'>1. Server status and real time information. <br>\n";
  ptr +="2. Essid beacon attack and hide our device server <br>\n";
  ptr +="3. help menu [you are here] <br>\n";
  ptr +="4. Push button panic attack mode <br>\n";
  ptr +="5. channels 6, 7, 11, 12, 4 attack mode <br>\n";
  ptr +="6. channels 1 to 9 attack mode <br>\n";
  ptr +="7. channels 10 to 14 attack mode <br>\n";
  ptr +="<br> \n";
  ptr +="<p>Note: remote droid is ready for action.</p>\n";
  ptr +="</font></body></html>\n";
  led_RX();
  return ptr;
}


// Shift out channels one by one
void nextChannel() {
  if (sizeof(channels) > 1) {
    uint8_t ch = channels[channelIndex];
    channelIndex++;
    if (channelIndex > sizeof(channels)) channelIndex = 0;

    if (ch != wifi_channel && ch >= 1 && ch <= 14) {
      wifi_channel = ch;
      wifi_set_channel(wifi_channel);
    }
  }
}

// Random MAC generator
void randomMac() {
  for (int i = 0; i < 6; i++){
     macAddr[i] = random(256);
     led_TX();
  }
}

void setup_attack() {
 led_working();
 // create empty SSID
  for (int i = 0; i < 32; i++)
    emptySSID[i] = ' ';

  // for random generator
  randomSeed(os_random());

  // set packetSize
  packetSize = sizeof(beaconPacket);
  if (wpa2) {
    beaconPacket[34] = 0x31;
  } else {
    beaconPacket[34] = 0x21;
    packetSize -= 26;
  }

  // generate random mac address
  randomMac();

  // start serial
  Serial.begin(115200);
  Serial.println();

  // get time
  currentTime = millis();

  // start WiFi
  WiFi.mode(WIFI_OFF);
  wifi_set_opmode(STATION_MODE);

  // Set to default WiFi channel 
  wifi_set_channel(channels[0]);
 
  // Display all saved WiFi SSIDs
  Serial.println("SSIDs:");
  int i = 0;
  int len = sizeof(ssids);
  while (i < len) {
    Serial.print((char)pgm_read_byte(ssids + i));
    i++;
  }

  Serial.println();
  Serial.println("Started.....");
  Serial.println();
led_working2();
}


void setup_attack2() {
 led_working();
  for (int i = 0; i < 32; i++)
    emptySSID[i] = ' ';
  randomSeed(os_random());
  packetSize = sizeof(beaconPacket);
    beaconPacket[34] = 0x21;
    packetSize -= 26;
  randomMac();
  Serial.begin(115200);
  Serial.println();
  currentTime = millis();
  WiFi.mode(WIFI_OFF);
  wifi_set_opmode(STATION_MODE);
  wifi_set_channel(channel1[6]);
  Serial.println();
  Serial.println("Started.....");
  Serial.println();
led_working2();
}

void setup_attack3() {
 led_working();
  for (int i = 0; i < 32; i++)
    emptySSID[i] = ' ';
  randomSeed(os_random());
  packetSize = sizeof(beaconPacket);
  beaconPacket[34] = 0x31;
  randomMac();
  Serial.begin(115200);
  Serial.println();
  currentTime = millis();
  WiFi.mode(WIFI_OFF);
  wifi_set_opmode(STATION_MODE);
  wifi_set_channel(channel2[1]);
  Serial.println();
  Serial.println("WEP ESSIDS lunching attack.");
  Serial.println();
  Serial.println("Started.....");
  Serial.println();
led_working2();
}

void setup_attack4() {
 led_working();
  for (int i = 0; i < 32; i++)
    emptySSID[i] = ' ';
  randomSeed(os_random());
  packetSize = sizeof(beaconPacket);
  beaconPacket[34] = 0x31;
  randomMac();
  Serial.begin(115200);
  Serial.println();
  currentTime = millis();
  WiFi.mode(WIFI_OFF);
  wifi_set_opmode(STATION_MODE);
  wifi_set_channel(channel3[10]);
  Serial.println();
  Serial.println("Started.....");
  Serial.println();
led_working2();
}
