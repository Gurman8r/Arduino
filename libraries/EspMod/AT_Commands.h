/* AT_Commands.h
*	Author: 	Melody Gurman
* 	Modified: 	11/11/2018
* * * * * * * * * * * * * * * */
#ifndef _AT_COMMANDS_H_
#define _AT_COMMANDS_H_

/*
	https://room-15.github.io/blog/2015/03/26/esp8266-at-command-reference/
*/

// Basic                                // ********************************************************************** //
#define AT_AT			"AT"			// Test AT startup
#define AT_GMR 			"AT+GMR"		// View version info
#define AT_RST 			"AT+RST"		// Restart module
#define AT_GSLP			"AT+GSLP"		// Enter deep-sleep mode
#define AT_ATE			"ATE"			// Enable / Disable echo
// Wifi Layer                           // ********************************************************************** //
#define AT_CWMODE		"AT+CWMODE"		// WIFI mode（station, AP, station + AP）
#define AT_CWJAP     	"AT+CWJAP"  	// Connect to AP
#define AT_CWLAP		"AT+CWLAP"  	// Lists available APs
#define AT_CWQAP     	"AT+CWQAP"  	// Disconenct from AP
#define AT_CWSAP     	"AT+CWSAP"  	// Configuration of softAP mode
#define AT_CWLIF		"AT+CWLIF"  	// List clients connected to ESP8266 softAP
#define AT_CWDHCP		"AT+CWDHCP"		// Enable/Disable DHCP
#define AT_CWSTAMAC		"AT+CWSTAMAC"	// Set MAC address of ESP8266 station
#define AT_CIPAPMAC		"AT+CIPAPMAC"	// Set MAC address of ESP8266 softAP
#define AT_CIPSTA		"AT+CIPSTA"		// Set IP address of ESP8266 station
#define AT_CIPAP		"AT+CIPAP"		// Set ip address of ESP8266 softAP
// TCPIP Layer                          // ********************************************************************** //
#define AT_CIPSTATUS 	"AT+CIPSTATUS"	// Information about connection
#define AT_CIPSTART  	"AT+CIPSTART"	// Establish TCP connection or register UDP port and start a connection
#define AT_CIPSEND   	"AT+CIPSEND"	// Send data
#define AT_CIPCLOSE  	"AT+CIPCLOSE"	// Close TCP or UDP connection
#define AT_CIFSR     	"AT+CIFSR"		// Get local IP address
#define AT_CIPMUX    	"AT+CIPMUX"		// Enable multiple connections or not
#define AT_CIPSERVER 	"AT+CIPSERVER"	// Configure as server
#define AT_CIPMODE		"AT+CIPMODE"	// Set transfer mode
#define AT_CIPSTO 		"AT+CIPSTO"		// Set server timeout
#define AT_CIUPDATE 	"AT+CIUPDATE"	// Update through network (*** Note: Read info on site above ***)
#define AT_IPD			"+IPD"			// Receive network data
// 										// ********************************************************************** //

#endif // !_AT_COMMANDS_H_