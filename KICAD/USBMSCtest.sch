EESchema Schematic File Version 4
LIBS:USBMSCtest-cache
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
L USBMSCtest-rescue:PIC24FJ64GB002-mylib20161223 U2
U 1 1 5D768C6E
P 5950 3950
F 0 "U2" H 5950 5643 60  0000 C CNN
F 1 "PIC24FJ64GB002" H 5950 5537 60  0000 C CNN
F 2 "" H 5950 4350 60  0000 C CNN
F 3 "" H 5950 5431 60  0000 C CNN
	1    5950 3950
	1    0    0    -1  
$EndComp
$Comp
L USBMSCtest-rescue:CN-ICSP-mylib20161223 CN1
U 1 1 5D77487C
P 2850 2950
F 0 "CN1" H 2758 3537 60  0000 C CNN
F 1 "CN-ICSP" H 2758 3431 60  0000 C CNN
F 2 "" H 2850 2800 60  0000 C CNN
F 3 "" H 2850 2800 60  0000 C CNN
	1    2850 2950
	1    0    0    -1  
$EndComp
$Comp
L USBMSCtest-rescue:+3.3V-mylib20161223 #PWR0101
U 1 1 5D77555C
P 3400 1950
F 0 "#PWR0101" H 3400 1875 30  0001 C CNN
F 1 "+3.3V" H 3400 2088 30  0000 C CNN
F 2 "" H 3400 1950 60  0000 C CNN
F 3 "" H 3400 1950 60  0000 C CNN
	1    3400 1950
	1    0    0    -1  
$EndComp
$Comp
L USBMSCtest-rescue:R-mylib20161223 R1
U 1 1 5D775DDB
P 3400 2300
F 0 "R1" H 3471 2338 40  0000 L CNN
F 1 "100K" H 3471 2262 40  0000 L CNN
F 2 "" V 3330 2300 30  0000 C CNN
F 3 "" H 3400 2300 30  0000 C CNN
	1    3400 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	2950 3250 3950 3250
Wire Wire Line
	3950 3150 3000 3150
Wire Wire Line
	3000 3150 3000 3100
Wire Wire Line
	3000 3100 2950 3100
$Comp
L USBMSCtest-rescue:GND-mylib20161223 #PWR0102
U 1 1 5D777015
P 3200 3000
F 0 "#PWR0102" H 3200 3000 30  0001 C CNN
F 1 "GND" H 3200 2930 30  0001 C CNN
F 2 "" H 3200 3000 60  0000 C CNN
F 3 "" H 3200 3000 60  0000 C CNN
	1    3200 3000
	1    0    0    -1  
$EndComp
$Comp
L USBMSCtest-rescue:+3.3V-mylib20161223 #PWR0103
U 1 1 5D7777B4
P 3200 2800
F 0 "#PWR0103" H 3200 2725 30  0001 C CNN
F 1 "+3.3V" H 3200 2938 30  0000 C CNN
F 2 "" H 3200 2800 60  0000 C CNN
F 3 "" H 3200 2800 60  0000 C CNN
	1    3200 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	2950 2800 3200 2800
Wire Wire Line
	2950 2950 3200 2950
$Comp
L USBMSCtest-rescue:C-mylib20161223 C4
U 1 1 5D778BE0
P 8500 2950
F 0 "C4" H 8615 2991 43  0000 L CNN
F 1 "0.1uF" H 8615 2910 43  0000 L CNN
F 2 "" H 8538 2800 30  0000 C CNN
F 3 "" H 8500 2950 60  0000 C CNN
	1    8500 2950
	1    0    0    -1  
$EndComp
$Comp
L USBMSCtest-rescue:+3.3V-mylib20161223 #PWR0104
U 1 1 5D77A9E7
P 8500 2550
F 0 "#PWR0104" H 8500 2475 30  0001 C CNN
F 1 "+3.3V" H 8500 2688 30  0000 C CNN
F 2 "" H 8500 2550 60  0000 C CNN
F 3 "" H 8500 2550 60  0000 C CNN
	1    8500 2550
	1    0    0    -1  
$EndComp
$Comp
L USBMSCtest-rescue:GND-mylib20161223 #PWR0105
U 1 1 5D77AD61
P 8500 3250
F 0 "#PWR0105" H 8500 3250 30  0001 C CNN
F 1 "GND" H 8500 3180 30  0001 C CNN
F 2 "" H 8500 3250 60  0000 C CNN
F 3 "" H 8500 3250 60  0000 C CNN
	1    8500 3250
	1    0    0    -1  
$EndComp
$Comp
L USBMSCtest-rescue:C-mylib20161223 C5
U 1 1 5D77BD46
P 8500 3750
F 0 "C5" H 8615 3791 43  0000 L CNN
F 1 "0.1uF" H 8615 3710 43  0000 L CNN
F 2 "" H 8538 3600 30  0000 C CNN
F 3 "" H 8500 3750 60  0000 C CNN
	1    8500 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	8500 2550 8500 2650
Wire Wire Line
	7950 2650 8500 2650
Connection ~ 8500 2650
Wire Wire Line
	8500 2650 8500 2800
Wire Wire Line
	7950 3150 8500 3150
Wire Wire Line
	8500 3150 8500 3100
Wire Wire Line
	8500 3250 8500 3150
Connection ~ 8500 3150
$Comp
L USBMSCtest-rescue:+3.3V-mylib20161223 #PWR0106
U 1 1 5D77E22E
P 8500 3550
F 0 "#PWR0106" H 8500 3475 30  0001 C CNN
F 1 "+3.3V" H 8500 3688 30  0000 C CNN
F 2 "" H 8500 3550 60  0000 C CNN
F 3 "" H 8500 3550 60  0000 C CNN
	1    8500 3550
	1    0    0    -1  
$EndComp
$Comp
L USBMSCtest-rescue:GND-mylib20161223 #PWR0107
U 1 1 5D77E87C
P 8500 3950
F 0 "#PWR0107" H 8500 3950 30  0001 C CNN
F 1 "GND" H 8500 3880 30  0001 C CNN
F 2 "" H 8500 3950 60  0000 C CNN
F 3 "" H 8500 3950 60  0000 C CNN
	1    8500 3950
	1    0    0    -1  
$EndComp
$Comp
L USBMSCtest-rescue:CN_USB_A-mylib20161223 P1
U 1 1 5D78493E
P 9150 4250
F 0 "P1" V 9132 4538 50  0000 L CNN
F 1 "CN_USB_A" V 9223 4538 50  0000 L CNN
F 2 "" V 9100 4150 50  0000 C CNN
F 3 "" V 9100 4150 50  0000 C CNN
	1    9150 4250
	0    1    1    0   
$EndComp
Wire Wire Line
	7950 4150 8850 4150
Wire Wire Line
	7950 4250 8850 4250
Wire Wire Line
	8500 3600 8350 3600
Wire Wire Line
	8350 3600 8350 3950
Wire Wire Line
	8500 3550 8500 3600
Connection ~ 8500 3600
$Comp
L USBMSCtest-rescue:+5.0V-mylib20161223 #PWR0108
U 1 1 5D78B604
P 8850 4000
F 0 "#PWR0108" H 8850 3925 30  0001 C CNN
F 1 "+5.0V" H 8850 4138 30  0000 C CNN
F 2 "" H 8850 4000 60  0000 C CNN
F 3 "" H 8850 4000 60  0000 C CNN
	1    8850 4000
	1    0    0    -1  
$EndComp
$Comp
L USBMSCtest-rescue:GND-mylib20161223 #PWR0109
U 1 1 5D78DD1E
P 8850 4400
F 0 "#PWR0109" H 8850 4400 30  0001 C CNN
F 1 "GND" H 8850 4330 30  0001 C CNN
F 2 "" H 8850 4400 60  0000 C CNN
F 3 "" H 8850 4400 60  0000 C CNN
	1    8850 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	8850 4350 8850 4400
Wire Wire Line
	8850 4000 8850 4050
$Comp
L USBMSCtest-rescue:CP-mylib20161223 C6
U 1 1 5D78F1BD
P 8500 4450
F 0 "C6" H 8618 4496 50  0000 L CNN
F 1 "10uF(Tantalum/Ceramic)" H 8618 4405 50  0000 L CNN
F 2 "" H 8538 4300 50  0000 C CNN
F 3 "" H 8500 4450 50  0000 C CNN
	1    8500 4450
	1    0    0    -1  
$EndComp
$Comp
L USBMSCtest-rescue:GND-mylib20161223 #PWR0110
U 1 1 5D78FD76
P 8500 4600
F 0 "#PWR0110" H 8500 4600 30  0001 C CNN
F 1 "GND" H 8500 4530 30  0001 C CNN
F 2 "" H 8500 4600 60  0000 C CNN
F 3 "" H 8500 4600 60  0000 C CNN
	1    8500 4600
	1    0    0    -1  
$EndComp
$Comp
L USBMSCtest-rescue:GND-mylib20161223 #PWR0111
U 1 1 5D7901BE
P 8350 4600
F 0 "#PWR0111" H 8350 4600 30  0001 C CNN
F 1 "GND" H 8350 4530 30  0001 C CNN
F 2 "" H 8350 4600 60  0000 C CNN
F 3 "" H 8350 4600 60  0000 C CNN
	1    8350 4600
	1    0    0    -1  
$EndComp
Wire Wire Line
	7950 4550 8350 4550
Wire Wire Line
	8350 4550 8350 4600
$Comp
L USBMSCtest-rescue:CN-UART-401-mylib20161223 CN2
U 1 1 5D791CA2
P 9000 5050
F 0 "CN2" H 8672 5072 60  0000 R CNN
F 1 "AE-FT234X" H 8672 5178 60  0000 R CNN
F 2 "" H 9000 4900 60  0000 C CNN
F 3 "" H 9000 4900 60  0000 C CNN
	1    9000 5050
	-1   0    0    -1  
$EndComp
$Comp
L USBMSCtest-rescue:GND-mylib20161223 #PWR0112
U 1 1 5D79431F
P 8850 4950
F 0 "#PWR0112" H 8850 4950 30  0001 C CNN
F 1 "GND" H 8850 4880 30  0001 C CNN
F 2 "" H 8850 4950 60  0000 C CNN
F 3 "" H 8850 4950 60  0000 C CNN
	1    8850 4950
	1    0    0    -1  
$EndComp
$Comp
L USBMSCtest-rescue:+5.0V-mylib20161223 #PWR0113
U 1 1 5D79480C
P 8850 4750
F 0 "#PWR0113" H 8850 4675 30  0001 C CNN
F 1 "+5.0V" H 8850 4888 30  0000 C CNN
F 2 "" H 8850 4750 60  0000 C CNN
F 3 "" H 8850 4750 60  0000 C CNN
	1    8850 4750
	1    0    0    -1  
$EndComp
$Comp
L USBMSCtest-rescue:+5.0V-mylib20161223 #PWR0114
U 1 1 5D7959E6
P 8350 5150
F 0 "#PWR0114" H 8350 5075 30  0001 C CNN
F 1 "+5.0V" H 8350 5288 30  0000 C CNN
F 2 "" H 8350 5150 60  0000 C CNN
F 3 "" H 8350 5150 60  0000 C CNN
	1    8350 5150
	1    0    0    -1  
$EndComp
$Comp
L USBMSCtest-rescue:+3.3V-mylib20161223 #PWR0115
U 1 1 5D798457
P 3400 4900
F 0 "#PWR0115" H 3400 4825 30  0001 C CNN
F 1 "+3.3V" H 3400 5038 30  0000 C CNN
F 2 "" H 3400 4900 60  0000 C CNN
F 3 "" H 3400 4900 60  0000 C CNN
	1    3400 4900
	1    0    0    -1  
$EndComp
$Comp
L USBMSCtest-rescue:C-mylib20161223 C3
U 1 1 5D7988C1
P 3400 5100
F 0 "C3" H 3515 5141 43  0000 L CNN
F 1 "0.1uF" H 3515 5060 43  0000 L CNN
F 2 "" H 3438 4950 30  0000 C CNN
F 3 "" H 3400 5100 60  0000 C CNN
	1    3400 5100
	1    0    0    -1  
$EndComp
Wire Wire Line
	2950 2650 3400 2650
Wire Wire Line
	3400 1950 3400 2050
Wire Wire Line
	3400 2550 3400 2650
Connection ~ 3400 2650
Wire Wire Line
	3400 2650 3950 2650
$Comp
L USBMSCtest-rescue:GND-mylib20161223 #PWR0116
U 1 1 5D79C230
P 3400 5300
F 0 "#PWR0116" H 3400 5300 30  0001 C CNN
F 1 "GND" H 3400 5230 30  0001 C CNN
F 2 "" H 3400 5300 60  0000 C CNN
F 3 "" H 3400 5300 60  0000 C CNN
	1    3400 5300
	1    0    0    -1  
$EndComp
Wire Wire Line
	3950 4950 3400 4950
Wire Wire Line
	3400 4950 3400 4900
Connection ~ 3400 4950
Wire Wire Line
	3400 5250 3400 5300
Wire Wire Line
	8500 4300 8350 4300
Wire Wire Line
	8350 4300 8350 4450
Wire Wire Line
	7950 4450 8350 4450
$Comp
L mylib20161223:Resonator_Small Y1
U 1 1 5D76A156
P 3300 4250
F 0 "Y1" V 2995 4200 50  0000 C CNN
F 1 "Resonator_Small" V 3086 4200 50  0000 C CNN
F 2 "" H 3275 4250 50  0001 C CNN
F 3 "~" H 3275 4250 50  0001 C CNN
	1    3300 4250
	0    1    1    0   
$EndComp
Wire Wire Line
	3400 4150 3950 4150
Wire Wire Line
	3400 4350 3400 4250
Wire Wire Line
	3400 4250 3950 4250
$Comp
L USBMSCtest-rescue:GND-mylib20161223 #PWR0117
U 1 1 5D76D502
P 3000 4300
F 0 "#PWR0117" H 3000 4300 30  0001 C CNN
F 1 "GND" H 3000 4230 30  0001 C CNN
F 2 "" H 3000 4300 60  0000 C CNN
F 3 "" H 3000 4300 60  0000 C CNN
	1    3000 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	3100 4250 3000 4250
Wire Wire Line
	3000 4250 3000 4300
$Comp
L USBMSCtest-rescue:R-mylib20161223 R2
U 1 1 5D76E5AF
P 3850 5500
F 0 "R2" H 3921 5538 40  0000 L CNN
F 1 "330" H 3921 5462 40  0000 L CNN
F 2 "" V 3780 5500 30  0000 C CNN
F 3 "" H 3850 5500 30  0000 C CNN
	1    3850 5500
	1    0    0    -1  
$EndComp
$Comp
L USBMSCtest-rescue:GND-mylib20161223 #PWR0118
U 1 1 5D76EB9F
P 3850 6350
F 0 "#PWR0118" H 3850 6350 30  0001 C CNN
F 1 "GND" H 3850 6280 30  0001 C CNN
F 2 "" H 3850 6350 60  0000 C CNN
F 3 "" H 3850 6350 60  0000 C CNN
	1    3850 6350
	1    0    0    -1  
$EndComp
$Comp
L mylib20161223:LED D1
U 1 1 5D76F2A3
P 3850 6050
F 0 "D1" V 3804 6128 50  0000 L CNN
F 1 "LED" V 3895 6128 50  0000 L CNN
F 2 "" H 3850 6050 60  0000 C CNN
F 3 "" H 3850 6050 60  0000 C CNN
	1    3850 6050
	0    1    1    0   
$EndComp
$Comp
L USBMSCtest-rescue:GND-mylib20161223 #PWR0119
U 1 1 5D7726C0
P 3850 4000
F 0 "#PWR0119" H 3850 4000 30  0001 C CNN
F 1 "GND" H 3850 3930 30  0001 C CNN
F 2 "" H 3850 4000 60  0000 C CNN
F 3 "" H 3850 4000 60  0000 C CNN
	1    3850 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3850 6250 3850 6350
Wire Wire Line
	3850 5850 3850 5750
Wire Wire Line
	3850 5250 3850 5150
Wire Wire Line
	3850 5150 3950 5150
Wire Wire Line
	3950 3950 3850 3950
Wire Wire Line
	3850 3950 3850 4000
$Comp
L mylib20161223:LM2931AZ-3.3_5.0 U1
U 1 1 5D7774F9
P 2100 6450
F 0 "U1" H 2100 6857 50  0000 C CNN
F 1 "LM2931AZ-3.3_5.0" H 2100 6766 50  0000 C CNN
F 2 "TO92-123" H 2100 6675 50  0000 C CIN
F 3 "" H 2100 6676 50  0000 C CNN
	1    2100 6450
	1    0    0    -1  
$EndComp
$Comp
L USBMSCtest-rescue:CP-mylib20161223 C2
U 1 1 5D779533
P 2700 6550
F 0 "C2" H 2818 6596 50  0000 L CNN
F 1 "10uF" H 2818 6505 50  0000 L CNN
F 2 "" H 2738 6400 50  0000 C CNN
F 3 "" H 2700 6550 50  0000 C CNN
	1    2700 6550
	1    0    0    -1  
$EndComp
$Comp
L USBMSCtest-rescue:CP-mylib20161223 C1
U 1 1 5D77AAF9
P 1500 6550
F 0 "C1" H 1618 6596 50  0000 L CNN
F 1 "10uF" H 1618 6505 50  0000 L CNN
F 2 "" H 1538 6400 50  0000 C CNN
F 3 "" H 1500 6550 50  0000 C CNN
	1    1500 6550
	1    0    0    -1  
$EndComp
$Comp
L USBMSCtest-rescue:GND-mylib20161223 #PWR0120
U 1 1 5D77AF1F
P 2100 6850
F 0 "#PWR0120" H 2100 6850 30  0001 C CNN
F 1 "GND" H 2100 6780 30  0001 C CNN
F 2 "" H 2100 6850 60  0000 C CNN
F 3 "" H 2100 6850 60  0000 C CNN
	1    2100 6850
	1    0    0    -1  
$EndComp
$Comp
L USBMSCtest-rescue:+3.3V-mylib20161223 #PWR0121
U 1 1 5D77B451
P 2800 6400
F 0 "#PWR0121" H 2800 6325 30  0001 C CNN
F 1 "+3.3V" H 2800 6538 30  0000 C CNN
F 2 "" H 2800 6400 60  0000 C CNN
F 3 "" H 2800 6400 60  0000 C CNN
	1    2800 6400
	1    0    0    -1  
$EndComp
$Comp
L USBMSCtest-rescue:+5.0V-mylib20161223 #PWR0122
U 1 1 5D77B99C
P 1400 6400
F 0 "#PWR0122" H 1400 6325 30  0001 C CNN
F 1 "+5.0V" H 1400 6538 30  0000 C CNN
F 2 "" H 1400 6400 60  0000 C CNN
F 3 "" H 1400 6400 60  0000 C CNN
	1    1400 6400
	1    0    0    -1  
$EndComp
Wire Wire Line
	1400 6400 1500 6400
Wire Wire Line
	1500 6400 1700 6400
Connection ~ 1500 6400
Wire Wire Line
	2500 6400 2700 6400
Wire Wire Line
	2700 6400 2800 6400
Connection ~ 2700 6400
Wire Wire Line
	1500 6700 2100 6700
Wire Wire Line
	2100 6850 2100 6700
Connection ~ 2100 6700
Wire Wire Line
	2700 6700 2100 6700
Wire Wire Line
	8900 4900 8850 4900
Wire Wire Line
	8850 4900 8850 4950
Wire Wire Line
	8350 5150 7950 5150
Wire Wire Line
	7950 4750 8700 4750
Wire Wire Line
	8650 4850 7950 4850
Wire Wire Line
	8500 3900 8500 3950
Wire Wire Line
	7950 3950 8350 3950
Wire Wire Line
	8850 4750 8900 4750
Wire Wire Line
	3200 2950 3200 3000
NoConn ~ 3950 2850
NoConn ~ 3950 2950
NoConn ~ 3950 3350
NoConn ~ 3950 3450
NoConn ~ 3950 4450
NoConn ~ 3950 4550
NoConn ~ 7950 4950
NoConn ~ 7950 3750
NoConn ~ 7950 3650
NoConn ~ 7950 3550
NoConn ~ 9250 4550
NoConn ~ 9350 4550
Wire Wire Line
	8700 5200 8900 5200
Wire Wire Line
	8700 4750 8700 5200
Wire Wire Line
	8650 4850 8650 5050
Wire Wire Line
	8650 5050 8900 5050
Text Notes 8450 5550 0    60   ~ 0
(RB8)RX - TX(AE-FT234X)\n(RB9)TX - RX(AE-FT234X)
$EndSCHEMATC
