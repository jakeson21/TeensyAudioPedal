EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Device:Rotary_Encoder_Switch SW1
U 1 1 6163A01D
P 5000 2500
F 0 "SW1" H 5000 2867 50  0000 C CNN
F 1 "Rotary_Encoder_Switch" H 5000 2776 50  0000 C CNN
F 2 "KnobBoard:KSA_Tactile_SPST_12mm" H 4850 2660 50  0001 C CNN
F 3 "~" H 5000 2760 50  0001 C CNN
	1    5000 2500
	1    0    0    -1  
$EndComp
$Comp
L Device:Rotary_Encoder_Switch SW2
U 1 1 6163AE43
P 5000 3500
F 0 "SW2" H 5000 3867 50  0000 C CNN
F 1 "Rotary_Encoder_Switch" H 5000 3776 50  0000 C CNN
F 2 "KnobBoard:KSA_Tactile_SPST_12mm" H 4850 3660 50  0001 C CNN
F 3 "~" H 5000 3760 50  0001 C CNN
	1    5000 3500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR01
U 1 1 6163BB1B
P 4100 4100
F 0 "#PWR01" H 4100 3850 50  0001 C CNN
F 1 "GND" H 4105 3927 50  0000 C CNN
F 2 "" H 4100 4100 50  0001 C CNN
F 3 "" H 4100 4100 50  0001 C CNN
	1    4100 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	4700 2500 4100 2500
Wire Wire Line
	4100 2500 4100 2900
Wire Wire Line
	4700 3500 4100 3500
Connection ~ 4100 3500
Wire Wire Line
	4100 3500 4100 3850
$Comp
L Connector:Conn_01x02_Female J2
U 1 1 6163E6DA
P 6600 2600
F 0 "J2" H 6628 2576 50  0000 L CNN
F 1 "Conn_01x02_Female" H 6628 2485 50  0000 L CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02_Pitch2.54mm" H 6600 2600 50  0001 C CNN
F 3 "~" H 6600 2600 50  0001 C CNN
	1    6600 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	6150 2700 6400 2700
Wire Wire Line
	6400 2600 6200 2600
Text Label 6250 2600 0    50   ~ 0
SW1
Text Label 6250 2700 0    50   ~ 0
SW2
Wire Wire Line
	6150 3400 6150 2700
Wire Wire Line
	5300 3400 6150 3400
Wire Wire Line
	5300 2400 6200 2400
Wire Wire Line
	6200 2400 6200 2600
Wire Wire Line
	5300 2600 5500 2600
Wire Wire Line
	5500 2600 5500 2900
Wire Wire Line
	5500 2900 4100 2900
Connection ~ 4100 2900
Wire Wire Line
	4100 2900 4100 3500
Wire Wire Line
	5300 3600 5500 3600
Wire Wire Line
	5500 3600 5500 3850
Wire Wire Line
	5500 3850 4100 3850
Connection ~ 4100 3850
Wire Wire Line
	4100 3850 4100 4100
$Comp
L Connector_Generic_MountingPin:Conn_01x04_MountingPin J1
U 1 1 616418F7
P 3300 3000
F 0 "J1" H 3222 3317 50  0000 C CNN
F 1 "Conn_01x04_MountingPin" H 3222 3226 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x04_Pitch2.54mm" H 3300 3000 50  0001 C CNN
F 3 "~" H 3300 3000 50  0001 C CNN
	1    3300 3000
	-1   0    0    -1  
$EndComp
Wire Wire Line
	3700 2900 3700 2400
Wire Wire Line
	3700 2400 4700 2400
Wire Wire Line
	3500 3000 4400 3000
Wire Wire Line
	4400 3000 4400 2600
Wire Wire Line
	4400 2600 4700 2600
Wire Wire Line
	3500 3100 4400 3100
Wire Wire Line
	4400 3100 4400 3400
Wire Wire Line
	4400 3400 4700 3400
Wire Wire Line
	4700 3600 3800 3600
Wire Wire Line
	3800 3600 3800 3200
Wire Wire Line
	3800 3200 3500 3200
Wire Wire Line
	3700 2900 3500 2900
Text Label 3550 2900 0    50   ~ 0
A1
Text Label 3550 3000 0    50   ~ 0
B1
Text Label 3550 3100 0    50   ~ 0
A2
Text Label 3550 3200 0    50   ~ 0
B2
$EndSCHEMATC
