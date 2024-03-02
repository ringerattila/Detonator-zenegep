

	#include <htc.h>
	#include "pic1687x.h"
	#include "pic.h"


//#define	DEBUG			// debug idején fogunk bizonyos lábakat
#ifdef DEBUG  // DEBUG esetén RB7, RB6 foglalt

	__CONFIG(XT & WDTDIS & LVPDIS & BORDIS &  DEBUGEN  );   

#else 

//	__CONFIG(XT & WDTDIS & LVPDIS & BORDIS & PWRTEN & DEBUGDIS&PROTECT&DPROT);  // kódvédett BORDIS->BOREN . /////PWRTEN->PWRDIS2010 09 22

	__CONFIG(XT & WDTDIS & LVPDIS & BORDIS & PWRTEN & DEBUGDIS&PROTECT&DPROT);  // kódvédett

//	__CONFIG(XT & WDTDIS & LVPDIS & WRTEN & BORDIS & DEBUGDIS); // nem kódvédett

#endif

#define	_XTAL_FREQ	4000000;

// #fuses  XT
// #fuses NOWDT

// #define delay (clock=4000000)
// unsigned char 	t;


/*******************************************************************************************
*    EEPROM inicializálás
*
*******************************************************************************************/

__EEPROM_DATA(0, 0, 0, 0, 0, 0, 0, 0);
__EEPROM_DATA(0, 0, 0, 0, 0, 0, 0, 0);

 // __EEPROM_DATA(sc1, sc2, sc3, 0, 0, 0, 0, 0);
//  __EEPROM_DATA(, sc2, sc3, 0, 0, 0, 0, 0);////////////////////////

__EEPROM_DATA(52, 53, 54, 55, 56, 57, 58, 88);  // 16-23
__EEPROM_DATA(57, 49, 53, 48, 68, 128, 98, 78); // 24 - 31
__EEPROM_DATA(53, 49, 48, 48, 48, 48, 108, 48);  // 32-39
__EEPROM_DATA( 48, 78, 48, 0xAA, 0xFF, 0xFF, 0xFF, 0xFF); // 40 - 47
__EEPROM_DATA( 0x4F, 0x02, 0, 0, 0, 0, 0, 0); // 48 -
__EEPROM_DATA( 0x4F, 0x02, 0, 0, 0, 0, 0, 0); // 56 -

__EEPROM_DATA( 0x4F, 0x02, 0, 0, 0, 0, 0, 0); // 64 -
__EEPROM_DATA( 0x4F, 0x02, 0, 0, 0, 0, 0, 0); // 72 - 
__EEPROM_DATA( 0x4F, 0x02, 0, 0, 0, 0, 0, 0); // 80 -
__EEPROM_DATA( 0x4F, 0x02, 0, 0, 0, 0, 0, 0); // 88 -
__EEPROM_DATA( 0x4F, 0x02, 0, 0, 0, 0, 0, 0); // 96
__EEPROM_DATA( 0x4F, 0x02, 0, 0, 0, 0, 0, 0); // 104
__EEPROM_DATA( 0x4F, 0x02, 0, 0, 0, 0, 0, 0); // 112
__EEPROM_DATA( 0x4F, 0x02, 0, 0, 0, 0, 0, 0); // 120

__EEPROM_DATA( 0x4F, 0x02, 0, 0, 0, 0, 0, 0); // 128
__EEPROM_DATA( 0x4F, 0x02, 0, 0, 0, 0, 0, 0); // 136
__EEPROM_DATA( 0x4F, 0x02, 0, 0, 0, 0, 0, 0); // 144
__EEPROM_DATA( 0x4F, 0x02, 0, 0, 0, 0, 0, 0); // 152
__EEPROM_DATA( 0x4F, 0x02, 0, 0, 0, 0, 0, 0); // 160
__EEPROM_DATA( 0x4F, 0x02, 0, 0, 0, 0, 0, 0); // 168
__EEPROM_DATA( 0x4F, 0x02, 0, 0, 0, 0, 0, 0); // 176
__EEPROM_DATA( 0x4F, 0x02, 0, 0, 0, 0, 0, 0); // 184

__EEPROM_DATA( 0x4F, 0x02, 0, 0, 0, 0, 0, 0); // 192
__EEPROM_DATA( 0x4F, 0x02, 0, 0, 0, 0, 0, 0); // 200
__EEPROM_DATA( 0x4F, 0x02, 0, 0, 0, 0, 0, 0); // 208
__EEPROM_DATA( 0x4F, 0x02, 0, 0, 0, 0, 0, 0); // 216
__EEPROM_DATA( 0x4F, 0x02, 0, 0, 0, 0, 0, 0); // 224
__EEPROM_DATA( 0x4F, 0x02, 0, 0, 0, 0, 0, 0); // 232
__EEPROM_DATA( 0, 0, 0, 0, 0, 0, 0, 0); 		// 240			// Counters
__EEPROM_DATA( 250, 0, 0, 0, 0, 0, 0,0); // 248			// Pass area

/******************************************************************************
* EEPROM memoriatáblázat
0 - 7 cím:  számláló állások, 1. sorozat
8 - 15 cím:  számláló állások, 2. sorozat

(1) STX (SETUP üzenet kezdete)
(2) 'H' (SETUP küldés parancskód)

Setup memoria kezdete, a byte-ok sorrendje: CH1, CH2, CH3, CH4, CH5
(3) 16 - CH1 - csatorna pénz értéke - lehetséges értékek '1'..'9', jelentése: ( 1-2-5-10-20-50-100-200-500) 
(4) 17 - CH2 - csatorna pénz értéke - lehetséges értékek '1'..'9', jelentése: ( 1-2-5-10-20-50-100-200-500) 
(5) 18 - CH3 - csatorna pénz értéke - lehetséges értékek '1'..'9', jelentése: ( 1-2-5-10-20-50-100-200-500) 
(6)	19 - CH4 - csatorna pénz értéke - lehetséges értékek '1'..'9', jelentése: ( 1-2-5-10-20-50-100-200-500) 
(7) 20 - CH5 - csatorna pénz értéke - lehetséges értékek '1'..'9', jelentése: ( 1-2-5-10-20-50-100-200-500) 
(8) 21 - CH6 - csatorna pénz értéke - lehetséges értékek '1'..'9', jelentése: ( 1-2-5-10-20-50-100-200-500) 
(9) 22 - minimum hangerő ( Value + '0')
(10) 23 - maximum hangerő ( Value + '0')

(11) 24 - MIC level ( Value + '0')
(12) 25 - PIN code ( '0'- nem, '1' - igeg)
(13) 26 - Fade Out
(14) 27 - Repeat Limit
(15) 28 - Autostep min
(16) 29 - Autostep max
(17) 30 - Backstep time
(18) 31 - Demo Time ('0' - nincs, demo,)

(19) 32 - Demo Fade Out
(20) 33 - Demo Top
(21) 34 - Anim
(22) 35 - Video Credit
(23) 36 - Karaoke Kredit
(24) 37 - Free Mode
(25) 38 - Procent
(26) 39 - Reserved

(27) 40 - 1 byte - gépben lévő kreditek száma
(28) 41 - 1 byte - hangerő aktuális értéke

(29) (CLC) (SETUP adathalmaz ellenőrzőkód)
(30) (ETX) (SETUP üzenet vége

42 - Booking_pointer - 1 byte
	(arra a cimre mutat, ahová a következő zárási adatokat kell irni. 
	Inicializáláskor 48 az értéke, mindig 8-cal növekszik, összesen 24 zárást tud tárolni.
	A zárások cimei: 1. 48
				2. 56
				3. 64
				4. 72
				5. 80
				6. 88
				7. 96
				8. 104
				9. 112
				10. 120
				11. 128
				12. 136
				13. 144
				14. 152
				15. 160
				16. 168
				17. 176
				18. 184
				19. 192
				20. 200
				21.	208
				22. 216
				23. 224
				24. 232
		A pointer értéke 232 után újra 48 lesz, az ezutáni zárások felülirják a korábbiakat.

43 -  0xAA vagy 0x55  (chip inicializáló ellenőrző byte, 0xAA, ha még nem futott a chip
44-- 47 címek üresek


	Zárások területe 8 byte zárásonként: 2 byte dátum, 6 byte: 30 bit hosszu számláló, 18 bit rövid számláló
	24 db zárás, összesen 192 byte

48 -  239 címeken a 24 db zárás

240 - 4 byte hosszu számláló aaktuális állása
244 - 4 byte rövid számláló aaktuális állása
248 - Pass_Ptr
249 - üres
250	 - 3*2 byte tárolt jelszó

*/

// #define setup_addr  		16; // seetup adathalmaz kezdete az EEPROM-ban
// #define setup_size 		26; // ennyi byte-ból áll a setup adathalmaz

const unsigned char counter1_addr = 240;
const unsigned char counter2_addr = 0;
const unsigned char counter3_addr = 8;
const unsigned char pass_area = 250;			// Pass terület kezdete
const unsigned char EEChkAddr = 43;			// Chip első futását ellenőrző byte címe
const unsigned char passptr_addr = 248;			// Pass pointer cime
const unsigned char setup_addr = 16;
const unsigned char setup_size = 26;
const unsigned char setup_credits_addr = 40;		// Erre a cimre kell irni a gépben lévő krediteket
const unsigned char booking_addr = 48;		// EEPROM-ban a könyvelés terület kezdete
const unsigned char last_booking_addr = 232;		// EEPROM-ban a zárás terület utolsó rekord kezdőcime
const unsigned char BKPointer = 42;		// EEPROM-ban a következő zárás kezdőcime
const unsigned int	Money_Val[] = { 0, 1, 2, 5, 10, 20, 50, 100, 200, 500 };	// érmevizsgáló pénzértékek
const unsigned int PrellDelay = 15;	// prell mentesítéshez ennyi ms-t várunk

/****************************************************************
*	Távvezérlő ismétlési ciklus
* Először 0-ról kell a RMC_First értéket elérni
* Az ismétlésekhez pedig az RMC_Repeat-ről kell az RMC_First értéket elérni
*****************************************************************************************/
const unsigned int RMC_First = 800;		// távvezérlő ismétlési ciklus - első nyomás késleltetése
const unsigned int RMC_Repeat = 500;		// távvezérlő ismétlési ciklus - ismétlési gyorsaság (First-Repeat)
const unsigned int Button_Repeat = 650;		// lapozó nyomógomb ismétlődési sebesség

const unsigned char ctmp[] = "0123456789";
// const unsigned char mess2[] = "0123456789ABCDEFGHIJKLMNO";

const unsigned char scode[] = "45411496v7.12";  // service code


#define	reset_vector	0x00	// reset vektor

// #define	m_kikap_en	0x081	// kikapcsolós megerositese (PC adja)
// #define	m_kikap_tilt	0x082	// kikapcsolósi kérelem elutasítása
// #define	m_shut_req	0x083	// kikapcsolósi kérelem KI gombbal


// ellenőrzőtt, jó üzenet kódok

#define	m_booking_rq				'z'		// setup adathalmaz kérése
#define	m_booking_send			'Z'		// setup adathalmaz kérése

#define	m_login					'i'		// PC bejelentkezése bekapcsolós után
#define	m_startup				'I'		// 'I' - inicializáló üzenet

#define	m_space					' '		// Szokoz karakter

#define	m_ch1_Pressed			'a'		// 0x041	// 'A' - érme a ch1 csatornán  (HW adja)
#define	m_ch1_Released			'A'		// 0x041	// 'A' - érme a ch1 csatornán  (HW adja)

#define	m_ch2_Pressed			'b'		//0x042	// 'B' - érme a ch2 csatornán  (HW adja)
#define	m_ch2_Released			'B'		//0x042	// 'B' - érme a ch2 csatornán  (HW adja)

#define	m_ch3_Pressed			'c'		// 0x043	// 'C' - érme a ch3 csatornán  (HW adja)
#define	m_ch3_Released			'C'		// 0x043	// 'C' - érme a ch3 csatornán  (HW adja)

#define	m_ch4_Pressed			'd'		//0x044	// 'D' - érme a ch4 csatornán  (HW adja)
#define	m_ch4_Released			'D'		//0x044	// 'D' - érme a ch4 csatornán  (HW adja)

#define	m_ch5_Pressed			'e'		//0x045	// 'E' - érme a ch5 csatornán  (HW adja)
#define	m_ch5_Released			'E'		//0x045	// 'E' - érme a ch5 csatornán  (HW adja)

#define	m_ch6_Pressed			'f'		//0x046	// 'F' - érme a ch6 csatornán  (HW adja)
#define	m_ch6_Released			'F'		//0x046	// 'F' - érme a ch6 csatornán  (HW adja)

#define	m_counters_rq			'g'		// setup adathalmaz kérése
#define	m_counters_send			'G'		// setup adathalmaz kérése

#define	m_setup_rq				'h'		// setup adathalmaz kérése
#define	m_setup_send			'H'		// setup adathalmaz küldése

#define	m_Vol_Pressed			'p'		// érmeajtó kinyitva üzenet -  belepes hangero allito modba
#define	m_Vol_Released			'P'		// érmeajtó bezárva	üzenet

#define	m_Service_Pressed		's'		// kulcsos kapcsoló elforditva	-  belépés service modba
#define	m_Service_Released		'S'		// kulcsos kapcsoló elengedve

#define	m_mute_on				'u'		// A PC kéri a némítás bekapcsolását
#define	m_mute_off				'U'		// A PC kéri a némítás kikapcsolását

#define	m_SLeft_Pressed			'w'		// Step_Left nyomógomb   megnyomva
#define	m_SLeft_Released			'W'		// Step_Left nyomógomb   elengedve

#define	m_SRight_Pressed			'o'		// Step_Right nyomógomb megnyomva
#define	m_SRight_Released		'O'		// Step_Right nyomógomb  elengedve

#define	m_GBack_Pressed			'v'		// Go_Back nyomógomb megnyomva
#define	m_GBack_Released		'V'		// Go_Back nyomógomb  elengedve

#define	m_GEnter_Pressed			'y'		// Go_Enter nyomógomb megnyomva
#define	m_GEnter_Released		'Y'		// Go_Enter nyomógomb  elengedve

#define	m_test_on				'X'		// A PC teszt módba lépett (nem számoljuk a pénzt)Go_Enter nyomógomb megnyomva
#define	m_test_off				'x'		// A PC kilépett a teszt módból 

#define	m_PLeft_Pressed			'j'		// Page_Left nyomógomb  (HW adja)
#define	m_PLeft_Released			'J'		// Page_Left nyomógomb  (HW adja)

#define	m_PRight_Pressed			't'
#define	m_PRight_Released		'T'

#define	m_Volmut_Pressed			'k'		// volume mute távvezérlőről
#define	m_Volmut_Released		'K'		// volume mute távvezérlőről

#define	m_Volup_Pressed			'l'		// volume up távvezérlőről
#define	m_Volup_Released			'L'		// volume up távvezérlőről

#define	m_Voldn_Pressed			'm'		// volume down távvezérlő gomb megnyomva
#define	m_Voldn_Released			'M'		// volume down távvezérlő gomb elengedve

#define	m_vol4b_Pressed			'n'		// távvezérlő 4. gomb
#define	m_vol4b_Released			'N'		// távvezérlő 4. gomb

#define	m_counter_reset			'@'		// kérés számlálók nullázására
#define	m_remote_credit			'#'		// távvezérlővel beadott kredit remote free módban

#define	m_shutdown_rq			'q'		// kikapcsolós jelzése (HW adja)
#define	m_shutdown				'Q'		// kikapcsolós jelzése (HW adja)

#define	m_errmsg				'%'		// kikapcsolós jelzése (HW adja)

//**********************************************************
// ASCII characters
//**********************************************************

#define		STX		0x02		// Start Of Text
#define		ETX		0x03		// End Of Text
#define		ACK		0x06		// Acknowledge
#define		NAK		0x15		// Negativ Acknowledge

// #define		bel		0x07		// bell
// #define		bs		0x08		// backspace
// #define		tab		0x09		// horizontal tab
// #define		lf		0x0A		// line feed
// #define		ff		0x0C		// form feed
// #define		cr		0x0D		// carriage return
// #define		xon		0x11		// transmit on
// #define		xoff		0x13		// transmit off
// #define		eof		0x1A		// end of file
// #define		esc		0x1B		// escape
// #define		sp		0x20		// space


//**********************************************************
// Aliases
//**********************************************************

#define		b_dip		RA0// IN1 - csoport bemenet
#define		b_but		RA1// IN2 - ra.1, nyomogombok bemenete
#define		b_vol		RA1// IN2 - ra.1, kapcsolo-csatornán jön ez is
#define		b_serv		RA1// service mod kapcsolo bemenet
#define		b_erm		RA5// IN3 - csoport bemenet

#define		b_kibe		RB0//

#define		oatxst		RC0		// régi hardver ATX kimenete
#define		opctap		RC4 		// régi hardver PCTAP kimenete


#ifdef DEBUG  // DEBUG esetén RB7, RB6 foglalt
	#define		atxst		RC0
	#define		pctap		RB5 // RB7-en lenne, a debugger miatt van itt

#else 
	#define		atxst		RB6
	#define		pctap		RB7 // RB7-en lenne, a debugger miatt van itt
#endif
// end of endif

#define		A0			RC1
#define		A1			RB1
#define		A2			RA3
#define		DAT			RA4
#define		CL1			RA2
#define		CL2			RC2
#define		SCL			RC3		// I2C busz órajel
#define		OK12V		RC5		// PC táp jele

#define		OA0			RB0		// régi vezérlő címzése
#define		OA1			RB1		// régi vezérlő címzése
#define		OA2			RB2		// régi vezérlő címzése
#define		ODAT		RB4		// régi vezérlő címzése
#define		OCL1		RB3		// régi vezérlő címzése
#define		ORST		RB5		// régi vezérlő címzése   most másra használjuk
#define		OIN1		RA0		// régi vezérlő bemenet csoport választás
#define		OIN2		RA1		// régi vezérlő bemenet csoport választás

#define _BufferOverrun	COM_Flags,0 // buffer overrun

// hangerő távir. gombjai
#define		rem_mut		RB4//
#define		rem_up		RB3// RA bit5
#define		rem_dn		RB2// RB bit7

//**********************************************************
// Konstansok definiálása
//**********************************************************

// const unsigned long servcode @ 0x1EF0 = 0x19783456; 

//**********************************************************
// Változók definiálása
//**********************************************************

// unsigned char rx_k;

typedef unsigned long DWORD;

typedef	enum	{ ok, serv, error, off }	BP_TYPE;
BP_TYPE		beep_type;

union	DateStore		// könyvelési adatok küldési formátuma
{
	unsigned int Zipped;
	struct
	{
	unsigned char	DLSB;				// összesen két byte-on a dátum
	unsigned char	DMSB;				//
	};
}

union	DateStore Date_Store;					// tömöritett dátum formátum

// union	BookSend	Book_Send;	// 

// union	booknum	Book_Store;	// 

union	bignum
{
	struct
	{
	unsigned long Hosszu;
	unsigned long	Rovid;
	};
	struct
	{
	unsigned int Hosszu_LInt;
	unsigned int Hosszu_MInt;
	unsigned int Rovid_LInt;
	unsigned int Rovid_MInt;
	};
	struct
	{
	unsigned char  H0;
	unsigned char  H1;
	unsigned char  H2;
	unsigned char  H3;
	unsigned char  R0;
	unsigned char  R1;
	unsigned char  R2;
	unsigned char  R3;
	};
	struct
	{
	unsigned char  cnumbyte[8];		// Hozzáférés ciklusszervezéssel
	};

};

union	bignum	Counter, Counter2, Counter3, Booking_Counter;
//, Book_Send;	// számláló állás tárolása

 
  union tVALTOZObits
{
       unsigned char x;
      struct {
          unsigned In_Msg: 1;
          unsigned Normal_Switchoff: 1;    		// =1, ha a PC magától állt le, =0, ha nem tud kikapcsolni
          unsigned Long_Switchoff: 1;			// =1, ha hosszabb idő múlva kapcsoljuk ki a PC-t (XP)     
          unsigned Monitor_Always_On: 1;		// =1, ha a monitort azonnal be kell kapcsolni
          unsigned Remote_2ch: 1;			// =1, ha kétcsatornás távvezérlő van a géphez
          unsigned Clear_Credit: 1;			// =1, ha törölni kell a krediteket (üzenet 'C' legyen, ha kell törölni, 'N' legyen, ha nem)
          unsigned Keyboard_Enable: 1;		// =1, ha a keyboard engedélyezve van ('K' legyen, ha használható a bill., 'N', ha nem)
          unsigned Remote_Present:1;
     	 };
        };
  
//   #define tVALTOZO tVALTOZObits.x

union tVALTOZObits SFbits; //Példányosítás


union ErmeBits
{
       unsigned char x;
      struct {
          unsigned CH1: 1;			// =1, ha a CH1 csatorna éppen pénzt vesz
          unsigned CH2: 1;    		// =1, ha a CH2 csatorna éppen pénzt vesz
          unsigned CH3: 1;			// =1, ha a CH3 csatorna éppen pénzt vesz
          unsigned CH4: 1;			// =1, ha a CH4 csatorna éppen pénzt vesz
          unsigned CH5: 1;			// =1, ha a CH5 csatorna éppen pénzt vesz
          unsigned CH6: 1;			// =1, ha a CH6 csatorna éppen pénzt vesz
          unsigned Serv_Mode: 1;	//  =1, ha a kulcsos kapcsoló el van forditva
          unsigned Erme_Ajto:1;		// =1, ha az érmeajtó nyitva van
     	 };
        };

union ErmeBits ERMbits = 0; //Példányosítás

   union ButBits
{
       unsigned char x;
      struct {
          unsigned Page_Left: 1;			// =1, ha a Page_Left gombot éppen nyomják
          unsigned Step_Left: 1;    		// =1, ha a Step_Left gombot éppen nyomják
          unsigned Return: 1;			// =1, ha a Return gombot éppen nyomják
          unsigned Enter: 1;				// =1, ha a Enter gombot éppen nyomják
          unsigned Step_Right: 1;		// =1, ha a Step_Right gombot éppen nyomják
          unsigned Page_Right: 1;		// =1, ha a Page_Right gombot éppen nyomják
          unsigned Ki_Be: 1;			//  =1, ha a Ki-Be gombot éppen nyomják
          unsigned Chip_First_Run:1;		// =1, ha a chip (PIC) először fut
     	 };
        };

union ButBits BUTbits = 0; 	// Példányosítás - 

   union BadButBits
{
       unsigned char x;
      struct {
          unsigned Page_Left: 1;			// =1, ha a Page_Left gomb be van ragadva
          unsigned Step_Left: 1;    		// =1, ha a Step_Left gomb be van ragadva
          unsigned Return: 1;			// =1, ha a Return gomb be van ragadva
          unsigned Enter: 1;				// =1, ha a Enter gomb be van ragadva
          unsigned Step_Right: 1;		// =1, ha a Step_Right gomb be van ragadva
          unsigned Page_Right: 1;		// =1, ha a Page_Right gomb be van ragadva
          unsigned MuteOff: 1;			//  =1, ha ujra szoljon az erosito
          unsigned t2ch:1;				// csak debug, jelezzük, hogy 2 csatornás a távvez.
     	 };
        };


union BadButBits BadButBits = 0; 	// Példányosítás - 


  union TVBits
{
       unsigned char x;
      struct {
          unsigned Mute: 1;			// =1, ha a TV0 gombját éppen nyomják
          unsigned Volup: 1;    		// =1, ha a TV1 gombját éppen nyomják
          unsigned Voldn: 1;		// =1, ha a TV2 gombját éppen nyomják
          unsigned Vol4b: 1;		// =1, ha a TV3 gombját éppen nyomják
          unsigned Fb4: 1;			// Nem használt
          unsigned PC_Test_Mode: 1;			// =1, ha a PC teszt módban van
          unsigned PC_On: 1;			// =1, ha a PC bejelentkezett, és kezdodik az adatcsere
          unsigned PC_Logged:1;			// =1, ha a PC bejelentkezését elfogadtuk
     	 };
        };
  
union	TVBits TVbits = 0; //Példányosítás


 //        if(VALTOZObits.Fb0)

// unsigned char	In_Msg;
// #define	In_Msg	SFbits.Fb2;
// #define	RX_Msg	Fbits.bit1;

//**********************************************************
// Serial port variables
//**********************************************************

#define  SER_BUFFER_SIZE 	64		// 64 byte-os a puffer
#define  SER_FIFO_MASK  (SER_BUFFER_SIZE-1)   // körbefordulás kezelésére

unsigned char rxtim=2;			// timeout
// soros port pufferek
unsigned char rxfifo[SER_BUFFER_SIZE];
unsigned char rxiptr, rxoptr;
unsigned char RX_Msg;				// soros vételi pufferbe beérkezett, fel nem dolgozott üzenetek száma
// unsigned char txfifo[SER_BUFFER_SIZE];
unsigned char txiptr, txoptr;
// volatile unsigned char hww;
unsigned char ser2_tmp;
unsigned char RX_Char;			// beérkezett karakter
// unsigned int tomb[20];
// unsigned char mess[32];	// kimeno uzenet tárolására
unsigned char DateStr[6] = {'0', '0', '1', '2', '3','1'};	// ASCII dátum form�tum
unsigned char Cmd;		// bejövő üzenet parancskódja
// unsigned int DateZipped;	// T�moritett dátum form�tum


unsigned char	local_CLC ;		// üzenet küldéshez
int	dcmut;					// ciklusváltozó - előjeles legyen!!
unsigned char  bnum;			// segédváltozó könyvelés küldéshez
unsigned char RH, Chk;			// Booking_ segedvaltozok
unsigned long mpl;				// ASCII konverzióhoz szorzó érték
unsigned int	RM_Count;		// távvezérlő folyamatos nyomva tartásának számlálója
unsigned  int	CH_Money[7];		// érmecsatornákhoz tartozó pénzérték
unsigned  int	CR_Pass;		// Counter_Reset Passsword
unsigned long CR_Mpl;			// Counter Reset segédszámláló
unsigned char  eeprom_irhato = 0;			// = 1, ha engedélyezzük az eeprom irást, biztonsági változó
unsigned char  Wrts;			// EEPROM írás hibajelzése: 0 esetén nincs hiba
unsigned char  ForcedMuteOff = 0;		// Erőltetett mute_off szakadt üzenet esetére


//**********************************************************
// rtc RS 372A variables
//**********************************************************
// unsigned char sec;
// unsigned char min;
// unsigned char hour;
// unsigned char day;
// unsigned char dw;
// unsigned char month;
// unsigned char year;

//**********************************************************
//EEPROM variables
//**********************************************************
unsigned char addr;
unsigned char value;
unsigned char value2;

//**********************************************************
//FLASH variables
//**********************************************************
// unsigned int address;
// unsigned int data;
// unsigned int data2;



//**********************************************************
// Altalános celu valtozok
//**********************************************************
unsigned char 	oldhw;				// hardware verzióját tartja nyilván, ha 1, akkor �j verzió

// unsigned char 	flags;				// jelzőbitek törölve
// unsigned char 	flags2;
// unsigned char 	buttons;
unsigned char 	minutes;				// időszámlálók törlése
unsigned char 	c2;	
unsigned char 	s1;	
unsigned char 	s2;	
// unsigned char 	hw_msg[16];			// hw által kóldött üzenetek
unsigned char 	s3;	
unsigned char 	Char_To_Send;		// küldendő karakter
// unsigned char 	Setup_Tomb[26];		//  PC setup értékeit tartalmazo tömb
unsigned char		Dec_str[10];			// számláló állás karakterei ASCII kódban
unsigned char		AddrL;			// Booking rutinokhoz

//**********************************************************
// BOOTLOADER variables
//**********************************************************

unsigned char 	Temp;	
unsigned char 	Temp1;	

// unsigned char 	ByteCount;		// adatrekord hossza
// #define		bnum	bnum;		// adatrekord hossza

unsigned char 	AddrH;	
//#define		_AddrH	addr;		// adatrekord hossza

// unsigned char 	AddrL;	
// #define		_AddrL	AddrL;		// adatrekord hossza

unsigned char 	RecType;	 		// rekord tipus
unsigned char 	HexDataH;	 
unsigned char 	HexDataL;	 
// unsigned char 	local_CLC;	// ellenőrző összeg

//******************************************
// automatikusan a 0x04 cimre kerül
// Interrupt kiszolgáló rutin
//*****************************************

void interrupt isr(void)  
{

/* isr code goes here */
di();		// további megszakitások tiltása

	if (RCIF)	// sorosan érkező karakter okozta az interruptot
		{
		TMR0=0;		//timeout törlés
		T0IE= 1;			//TIMER0 megszakitas engedélyezés 
		T0IF= 0;			//esetlegesTIMER0 megszakitas törlés	
		RX_Char = RCREG;
		if ((!SFbits.In_Msg) && (RX_Char == STX))
			{
			SFbits.In_Msg = 1;		// STX-et kaptunk, kezdodik az üzenet
			}
		if (SFbits.In_Msg)
			{
			rxfifo[rxiptr] = RX_Char;
			rxiptr++;
			rxiptr &= SER_FIFO_MASK;			// esetleges körbefordulás kezelése
//			ser2_tmp = (rxiptr+1) & SER_FIFO_MASK;
//			if (ser2_tmp != rxoptr)
//			rxiptr=ser2_tmp;
			if  (RX_Char == ETX)
				{
				SFbits.In_Msg = 0;		// ETX jött, kész az üzenet
				RX_Msg ++;			// üzenetek száma növekedjen
				T0IE= 0;			//TIMER0 megszakitas tiltas ////////////////////////////////////////
		//	RX_Msg = 1;		// 
				}
			}

		RCIF = 0;			// int flag törlése, karakter lekezelve
	
		} // if rcif


//timer0 megszakítás		65,536 ms ez a max alap
		if (T0IF)
		{
	
		T0IE= 0;
		RX_Char = ETX;					// lezárjuk az üzenetet 
		rxfifo[rxiptr] = RX_Char;
		rxiptr++;	
		rxiptr &= SER_FIFO_MASK;			// esetleges körbefordulás kezelése
		SFbits.In_Msg = 0;		
		RX_Msg ++;			// üzenetek száma növekedjen
		ForcedMuteOff = 1;	// jelezzuk az eseménykezelőnek, hogy mute off legyen

		T0IF=0;
		T0IE= 0;			//TIMER0 megszakitas engedélyezés 
		}



// timer2 megszakítás		262.144 ms ez a max alap????????
//		if (TMR2IF)
//		{
//		TMR2IF=0;
//		}

ei();									// megszakitas engedelyezes


} // interrupt handler


// #PRAGMA PSECT VALAMI



#pragma code init2 = 0x1000

// volatile const char portv @ 0x045
//	asm("psect absdata, abs, ovrld");
//	asm("org 1000h");

//	psect absdata,abs,ovrld
//	org 1000h

void Delayms (int dms);


void pctap_be(void )
{	
if	(oldhw)
	{
	opctap = 1;
	} else
	{
	pctap = 1;
	}
}

/* ****************************************************
*   Detonator 2010 régi és uj hardware
**
*  hardware specifikus rutinok gyűjteménye
******************************************************/


int Init_New_Ports(void)			//uj hw
{	
	PORTB = 0;
	PORTC = 0;
	PORTA = 0;
	ADCON0 = 0;		// AD converter letiltva
	ADCON1 = 6;		// minden porta láb digitális
	CL1 = 1;		// H szintben legyenek induláskor
	CL2 = 1;


/*    	bit 7 RBPU
		0 = Pull-up enabled
		1 = Pull-up disabled
	bit 6 INTEDG
	bit 5 T0CS: TMR0 Clock Source Select bit
		1 = Transition on T0CKI pin
		0 = Internal instruction cycle clock (CLKOUT)
	bit 4 T0SE: TMR0 Source Edge Select bit
		1 = Increment on high-to-low transition on T0CKI pin
		0 = Increment on low-to-high transition on T0CKI pin
	bit 3 PSA: Prescaler Assignment bit
		1 = Prescaler is assigned to the WDT
		0 = Prescaler is assigned to the Timer0 module
	bit 2-0 PS2:PS0: Prescaler Rate Select bits
*/

	OPTION = 0b00000000;

/*
Először a PORTC-t bemenetre állítjuk, és a RC5 olvasásával eldöntjük, hogy régi, vagy új a HW.
Ha RC5 = 0, akkor új verziós a hardware (AQ-403, 404), 
ha RC5 = 1, akkor régi verzió (DET-02)

UJ HW PORTC kiosztás:
	PORTC, 7 : RX - bemenet
	PORTC, 6 : TX - kimenet
	PORTC, 5 : OK12V - bemenet
	PORTC, 4 : SDA - bemenet
	PORTC, 3 :  SCL - kimenet
	PORTC, 2 :  CL2 - kimenet
	PORTC, 1 :  A0 - kimenet
	PORTC, 0 : ATXST - kimenet

*/


#ifdef DEBUG  // DEBUG esetén RC0-t használjuk

	TRISC = 0b10110000;

#else 

	TRISC = 0b10110001;

#endif




/*
	PORTA, 7 : Nincs
	PORTA, 6 : Nincs
	PORTA, 5 : IN3 - bemenet
	PORTA, 4 : DAT - kimenet
	PORTA, 3 :  A2 - kimenet
	PORTA, 2 :  CL1 - kimenet
	PORTA, 1 : ,		 IN2 - bemenet
	PORTA, 0 : b_dip,	 IN1 - bemenet

*/
	TRISA = 0b00100011;

/*
	PORTB, 7 : Debugger
	PORTB, 6 : Debugger
	PORTB, 5 : PCTAP - kimenet
	PORTB, 4 : REM_MUT - bemenet távvezérlőről
	PORTB, 3 :  REM_UP - bemenet távvezérlőről
	PORTB, 2 :  REM_DN - bemenet távvezérlőről
	PORTB, 1 :  A1 - kimenet
	PORTB, 0 : ON - bemenet

*/


#ifdef DEBUG  // DEBUG esetén RB7, RB6 foglalt

	TRISB = 0b00011101;

#else 

	TRISB = 0b00111101;

#endif


}		// uj hw init vege


int Init_Old_Ports(void)		// regi hw
{	
	PORTB = 0b00000000;
	PORTC = 0b00100000;		// a buzzer legyen H, hogy ne szoljon, a többi 0
	PORTA = 0;
	ADCON0 = 0;		// AD converter letiltva
	ADCON1 = 6;		// minden porta láb digitális
	OCL1 = 1;			// H szintben legyenek induláskor


#ifdef DEBUG  // DEBUG esetén RB5 foglalt


#else 

	ORST = 0;		//	
	Delayms(1);		// 	Reset a kimeneti IC-nek
	ORST = 1;		//	Reset impulzust adunk ki
	Delayms(1);
	ORST = 0;		//	RB5 az RST jel, legyen állandó L szinten

#endif



/*    	bit 7 RBPU
		0 = Pull-up enabled
		1 = Pull-up disabled
	bit 6 INTEDG
	bit 5 T0CS: TMR0 Clock Source Select bit
		1 = Transition on T0CKI pin
		0 = Internal instruction cycle clock (CLKOUT)
	bit 4 T0SE: TMR0 Source Edge Select bit
		1 = Increment on high-to-low transition on T0CKI pin
		0 = Increment on low-to-high transition on T0CKI pin
	bit 3 PnSA: Prescaler Assignment bit
		1 = Prescaler is assigned to the WDT
		0 = Prescaler is assigned to the Timer0 module
	bit 2-0 PS2:PS0: Prescaler Rate Select bits
*/

	OPTION = 0b00000000;

/*
Először a PORTC-t bemenetre állítjuk, és a RC5 olvasásával eldöntjük, hogy régi, vagy új a HW.
Ha RC5 = 0, akkor új verziós a hardware (AQ-403, 404), 
ha RC5 = 1, akkor régi verzió (DET-02)

regi HW PORTC kiosztás:
	PORTC, 7 : RX - bemenet
	PORTC, 6 : TX - kimenet
	PORTC, 5 : BUZZER -kimenet
	PORTC, 4 : PC TAP - kimenet
	PORTC, 3 : AMP - kimenet
	PORTC, 2 : MONITOR - kimenet
	PORTC, 1 :  MUTE - kimenet
	PORTC, 0 : ATXST - kimenet

*/

	TRISC = 0b10000000;

/*
	PORTA, 7 : Nincs
	PORTA, 6 : Nincs
	PORTA, 5 : TV1 - bemenet
	PORTA, 4 : NOP/MOST TV2 - bemenet///////////////////////////////////////////////////////
	PORTA, 3 :  ERMETILTAS - kimenet
	PORTA, 2 :  TV0 - bemenet
	PORTA, 1 : KAPCSOLOK+MECH+BEK.IN2 - bemenet
	PORTA, 0 : ERME+TSC+BEK,	 IN1 - bemenet

*/
	TRISA = 0b00110111;


/*
	PORTB, 7 : TV2 / MOST DEBUGGER -bemenet
	PORTB, 6 : TST/ most Debugger -bemenet
	PORTB, 5 : RESET - kimenet most fix a reset TST -bemenet a vegen beallitani !!!!!!!!!!!!!!!
	PORTB, 4 : ODAT - kimenet
	PORTB, 3 :  OCL1-kimenet
	PORTB, 2 :  OA2-kimenet
	PORTB, 1 :  OA1-kimenet
	PORTB, 0 : OA0- kimenet

*/


#ifdef DEBUG  // DEBUG esetén RB7, RB6 foglalt

	TRISB = 0b00100000;

#else 

	TRISB = 0b11000000;
	ORST = 0;		//	RB5 az RST jel, legyen állandó L szinten/*/*/*/*/*//

#endif

}		//regi hw init vege


void timer_init(void)
{//Timer 1 init

	T1CON = 0b00110001;	//TIMER1_ON_16BIT_ T0CKI _ ELO OSZTO 8_FOSC/4
	TMR1IE=0;			//TIMER1 megszakitas engedélyezés - most nem


		  /*bit 7-6 Unimplemented: Read as '0'
		bit 5-4 T1CKPS1:T1CKPS0: Timer1 Input Clock Prescale Select bits
		11 = 1:8 Prescale value
		10 = 1:4 Prescale value
		01 = 1:2 Prescale value
		00 = 1:1 Prescale value
		bit 3 T1OSCEN: Timer1 Oscillator Enable Control bit
		1 = Oscillator is enabled
		0 = Oscillator is shut-off (the oscillator inverter is turned off to eliminate power drain)
		bit 2 T1SYNC: Timer1 External Clock Input Synchronization Control bit
		When TMR1CS = 1:
		1 = Do not synchronize external clock input
		0 = Synchronize external clock input
		When TMR1CS = 0:
		This bit is ignored. Timer1 uses the internal clock when TMR1CS = 0.
		bit 1 TMR1CS: Timer1 Clock Source Select bit
		1 = External clock from pin RC0/T1OSO/T1CKI (on the rising edge)
		0 = Internal clock (FOSC/4)
		bit 0 TMR1ON: Timer1 On bit
		1 = Enables Timer1
		0 = Stops Timer1
		*/
// Timer 0 init
	T0CS=0;	//Internal instruction cycle clock
	PSA=0;	//Prescaler is assigned to the Timer0 module
	PS0=1;	//Prescaler 256
	PS1=1;
	PS2=1;
	T0IE= 0;			//TIMER0 megszakitas engedélyezés - most nem
// Timer 2 init
	T2CON=0b01111111;
	PR2=0;
	TMR2IE= 0;		//TIMER2 megszakitas engedélyezés - most nem
/*
bit 7 Unimplemented: Read as '0'
bit 6-3 TOUTPS3:TOUTPS0: Timer2 Output Postscale Select bits
0000 = 1:1 Postscale
0001 = 1:2 Postscale
0010 = 1:3 Postscale
1111 = 1:16 Postscale
bit 2 TMR2ON: Timer2 On bit
1 = Timer2 is on
0 = Timer2 is off
bit 1-0 T2CKPS1:T2CKPS0: Timer2 Clock Prescale Select bits
00 = Prescaler is 1
01 = Prescaler is 4
1x = Prescaler is 16*/
}

/*************************************************************
    USART Inicializ�l�s
*************************************************************/

#define	FOSC	4000000		// 4 MHz kvarc
// számítás képlete SPBRG = 1 esetén: Baud_Rate = FOSC/(16(X+1));
// #define Baud_Rate  9600

// const unsigned char SPBRG_Value = (FOSC/(16 * Baud_Rate)) - 1;

const unsigned int Baud_Rate = 19200;    //

const unsigned long SPBRG_High = 250000;    // Ha BRG=1 (Hi-Speed) lesz 4 MHZ órajel
const unsigned long SPBRG_Low = 625000;    // Ha BRG=0 (Lo-Speed) lesz

// const unsigned char SPBRG_Value = (4000000/16/Baud_Rate) - 1;
unsigned char SPBRG_Value;

void		init_usart(void)
{

//; bd_19200	equ	d'13'
#define	bd_9600		26
#define	bd_4800		52		//   BRGH=1 esetén
#define	bd_2400		25		//	BRGH=0 esetén
//; bd_1200		equ	d'207'

// SPBRG Baud rate generator register

		SPBRG_Value = SPBRG_High / Baud_Rate - 1;
		SPBRG = SPBRG_Value;

//		SPBRG = bd_2400
//		SPBRG = bd_4800

		TXIE = 0;		// később engedélyezzük, még ne kérjen it-t

// TXSTA transmit registers setup
//		; TXSTA register
//		; bit7 - x - don't care at async mode
//		; bit6 - 0 - select 8 bit transmission
//		; bit5 - 1 - transmit enabled
//		; bit4 - 0 - Async mode select
//		; bit3 - x - unimplemented
//		; bit2 - 1 - High speed select (BRGH)
//		; bit1 - 0 - Transmit register is empty
//		; bit0 - x - 9th bit

		TXSTA = 0b00100100;		// BRGH = 1

//		TXSTA = 0b00100000;		// BRGH = 0

// RCSTA receive register setup
//		; RCSTA register
//		; bit7 - 1 - serial port enabled
//		; bit6 - 0 - select 8 bit reception
//		; bit5 - x - don't care in async mode
//		; bit4 - 1 - enables continuous receive
//		; bit3 - x - unimplemented
//		; bit2 - 0 - Framing error bit
//		; bit1 - 0 - Overrun error
//		; bit0 - x - 9th bit

		RCSTA = 	0b10010000;		// regiszter kezdőérték
	
		RCIE = 1;
		PEIE = 1;		// usart kérhet megszakitast

txiptr = txoptr = rxiptr = rxoptr = 0;
// txiptr = txoptr = 0;	// pointerek kezdőértéke

} // init_usart


void bor(void)
	{

//	BOR=1;
//	asm("nop");
//	asm("nop");
//	asm("nop");
	if(!BOR&&POR&&PD&&TO)
		{
	while(1)
{
//		BOR=1;
//		BOR=0;
//		BOR=1;
				atxst = 1;
//		Delayms(1000);
}
		}

	}


void	erme_lam_on(void)
{

if	(oldhw)
	{
	// régi hw-ben nincs érmeajtó lámpa kezelés
	} else
	{
		A2=1;
 		A1=1;
		A0=0;
		DAT=1;
		CL1=0;
		CL1=1;
	}

}
  void	erme_lam_off(void)
{
if 	(oldhw)
	{
	// régi hw-ben nincs érmeajtó lámpa kezelés
	} else
	{
		A2=1;
		A1=1;
		A0=0;
		DAT=0;
		CL1=0;
		CL1=1;
	}
}	

void	buzz_on(void)
{
if	(oldhw)
	{
		RC5 = 0;
	} else
	{
			A0=1;
			A1=1;
			A2=1;
			DAT=1;
			CL2=0;
			CL2=1;
	}		
}

void	buzz_off(void)
{
if	(oldhw)
	{
	RC5 = 1;
	} else
	{
			A0=1;
			A1=1;
			A2=1;
			DAT=0;
			CL2=0;
			CL2=1;
	}
}


void	err_led_on(void)
{
if	(oldhw)
	{
	// régi hardware-en nincs error led
	} else
	{
		A2 = 1;
		A1 = 0;
		A0 = 1;
		DAT = 1;
		CL1 = 0;
		CL1 = 1;
	}
}


void err_led_off(void)	
{
if	(oldhw)
	{
	// régi hardware-en nincs error led
	} else
	{
		A2=1;
		A1=0;
		A0=1;
		DAT=0;
		CL1=0;
		CL1 = 1;
	}
}

void sleep_led_on	(void)
{
if	(oldhw)
	{			// a régi hw-n a piros led
		OA2=1;
		OA1=0;
		OA0=1;
		ODAT=1;
		OCL1=1;
		OCL1=0;
		OCL1=1;
	} else
		{
		A2=1;
		A1=0;
		A0=0;
		DAT=1;
		CL1=0;
		CL1=1;
	}
}

void sleep_led_off(void)
{
if	(oldhw)
	{			// a régi hw-n a piros led
		OA2=1;
		OA1=0;
		OA0=1;
		ODAT=0;
		OCL1=1;
		OCL1=0;
		OCL1=1;
	} else
		{
		A2=1;
		A1=0;
		A0=0;
		DAT=0;
		CL1=0;
		CL1=1;
		}
}


void run_led_on(void)
{
if	(oldhw)
	{			// a régi hw-n a sarga led
		OA2=1;
		OA1=1;
		OA0=0;
		ODAT=1;
		OCL1=1;
		OCL1=0;
		OCL1=1;
	} else
		{
		A2=0;
		A1=0;
		A0=0;
		DAT=1;
		CL1=1;
		CL1=0;
		CL1=1;
		}
}

void run_led_off(void)
{
if	(oldhw)
	{			// a régi hw-n a sarga led
		OA2=1;
		OA1=1;
		OA0=0;
		ODAT=0;
		OCL1=1;
		OCL1=0;
		OCL1=1;
	} else
		{
		A2=0;
		A1=0;
		A0=0;
		DAT=0;
		CL1=0;
		CL1=1;
		}
}

void	kibe_lam_on(void)
{
if	(oldhw)
	{
	// régi hardware-en nincs ki-be lámpa
	} else
		{
		A2=0;
		A1=0;
		A0=1;
		DAT=1;
		CL1=0;
		CL1=1;
		}			
}

void kibe_lam_off(void)
{
if	(oldhw)
	{
	// régi hardware-en nincs ki-be lámpa
	} else
		{
		A2=0;
		A1=0;
		A0=1;
		DAT=0;
		CL1=0;
		CL1=1;
		}		
}


void	counter_on(void)
{
if	(oldhw)
	{
	// régi hardware-en nincs counter
	} else
		{
		A2=0;
		A1=1;
		A0=0;
		DAT=1;
		CL1=0;
		CL1=1;
		}		
}

void counter_off(void)
{
if	(oldhw)
	{
	// régi hardware-en nincs counter
	} else
		{
		A2=0;
		A1=1;
		A0=0;
		DAT=0;
		CL1=0;
		CL1=1;
		}		
}

void montap_ki(void)
{
if	(oldhw)
		{
		RC2 = 0;
		} else
		{
			A2=1;
			A1=1;
			A0=1;
			DAT=0;
			CL1=0;
			CL1=1;
			}
}

void montap_be(void)
{
if	(oldhw)
		{
		RC2 = 1;
		} else
		{
		A2=1;
		A1=1;
		A0=1;
		DAT=1;
		CL1=0;
		CL1=1;
		}		
}


void amptap_be(void) 
{
if	(oldhw)
		{
		RC3 = 1;
		} else
		{
		A2=0;
		A1=1;
		A0=1;
		DAT=1;
		CL1=0;
		CL1=1;
		}
}

void amptap_ki(void )
{
if	(oldhw)
		{
		RC3 = 0;
		} else
		{
		A2=0;
		A1=1;
		A0=1;
		DAT=0;
		CL1=0;
		CL1=1;
		}
}

void mute_ki(void )
{
if	(oldhw)
	{
	RC1 = 1;
	} else
		{
		A2=0;
		A1=0;
		A0=1;
		DAT=1;
		CL2=0;
		CL2=1;
		}		
}

void mute_be(void )
{
if	(oldhw)
	{
	RC1 = 0;
	} else
	{
		A2=0;
		A1=0;
		A0=1;
		DAT=0;
		CL2=0;
		CL2=1;
		}
}


void ermtilt_be(void)
{
if	(oldhw)
	{
	RA3 = 1;
	} else
		{
		A2=0;
		A1=0;
		A0=0;
		DAT=0;
		CL2=0;
		CL2=1;
	}
}


void ermtilt_ki	(void )
{
if	(oldhw)
	{
	RA3 = 0;
	} else
		{	
		A2=0;
		A1=0;
		A0=0;
		DAT=1;
		CL2=0;
		CL2=1;
	}
}

void vent_lassu(void )
{
if	(oldhw)
	{
	// régi hardware-en nincs ventilátor vezérlés
	} else
		{
		A2=1;
		A1=1;
		A0=0;
		DAT=0;
		CL2=0;
		CL2=1;
		}
}

void vent_gyors(void )
{
if	(oldhw)
	{
	// régi hardware-en nincs ventilátor vezérlés
	} else
		{
		A2=1;
		A1=1;
		A0=0;
		DAT=1;
		CL2=0;
		CL2=1;
		}
}

void ron_be(void )
{
if	(oldhw)
	{
	// régi hardware-en nincs kimenő távvezérlés
	} else
		{
		A2=1;
		A1=0;
		A0=0;
		DAT=0;
		CL2=0;
		CL2=1;
		}
}

void ron_ki(void )
{
if	(oldhw)
	{
	// régi hardware-en kimenő távvezérlés
	} else
		{
		A2=1;
		A1=0;
		A0=0;
		DAT=1;
		CL2=0;
		CL2=1;
		}
}

void roff_be(void )
{
if	(oldhw)
	{
	// régi hardware-en nincs kimenő távvezérlés
	} else
		{
		A2=1;
		A1=0;
		A0=1;
		DAT=0;
		CL2=0;
		CL2=1;
		}
}

void roff_ki(void )
{
if	(oldhw)
	{
	// régi hardware-en nincs kimenő távvezérlés
	} else
		{
		A2=1;
		A1=0;
		A0=1;
		DAT=1;
		CL2=0;
		CL2=1;
		}
}



void pctap_ki(void )
{	
if	(oldhw)
	{
	opctap = 0;
	} else
	{
	pctap = 0;
	}
}


unsigned char	PC12V(void )
{	
	c2 = 1;		// vegyük úgy, hogy meg van nyomva

if	(oldhw)
	{
	c2 = 1;			// a régi HW-nél nincs ilyen
	} else
	{
	c2 =	RC5;		// uj hw-nél rc5=1, ha beindult a 12V tápegység
	}
	return c2;
}


void atxst_be(void )
{	
if	(oldhw)
	{
	oatxst = 1;
	} else
	{
	atxst = 1;
	}
}

void atxst_ki(void )
{	
if	(oldhw)
	{
	oatxst = 0;
	} else
	{
	atxst = 0;
	}
}



// csak a régi vezérlőnél van ilyen
// BLAM6
void PageLeftLam_be(void)
{
if	(oldhw)
		{
		OA2=1;
		OA1=0;
		OA0=0;
		ODAT=1;
		OCL1=1;
		OCL1=0;
		OCL1=1;
	}
}

void PageLeftLam_ki(void)
{
if	(oldhw)
		{
		OA2=1;
		OA1=0;
		OA0=0;
		ODAT=0;
		OCL1=1;
		OCL1=0;
		OCL1=1;
	}
}

void StepLeftLam_be(void)
{
if	(oldhw)
		{
		OA2=0;
		OA1=1;
		OA0=1;
		ODAT=1;
		OCL1=1;
		OCL1=0;
		OCL1=1;
	}
}

void StepLeftLam_ki(void)
{
if	(oldhw)
		{
		OA2=0;
		OA1=1;
		OA0=1;
		ODAT=0;
		OCL1=1;
		OCL1=0;
		OCL1=1;
	}
}

void StepRightLam_be(void)
{
if	(oldhw)
		{
		OA2=0;
		OA1=0;
		OA0=0;
		ODAT=1;
		OCL1=1;
		OCL1=0;
		OCL1=1;
	}
}

void StepRightLam_ki(void)
{
if	(oldhw)
		{
		OA2=0;
		OA1=0;
		OA0=0;
		ODAT=0;
		OCL1=1;
		OCL1=0;
		OCL1=1;
	}
}

void PageRightLam_be(void)
{
if	(oldhw)
		{
		OA2=1;
		OA1=1;
		OA0=1;
		ODAT=1;
		OCL1=1;
		OCL1=0;
		OCL1=1;
	}
}

void PageRightLam_ki(void)
{
if	(oldhw)
		{
		OA2=1;
		OA1=1;
		OA0=1;
		ODAT=0;
		OCL1=1;
		OCL1=0;
		OCL1=1;
	}
}

void EnterLam_be(void)
{
if	(oldhw)
		{
		OA2=0;
		OA1=0;
		OA0=1;
		ODAT=1;
		OCL1=1;
		OCL1=0;
		OCL1=1;
	}
}

void EnterLam_ki(void)
{
if	(oldhw)
		{
		OA2=0;
		OA1=0;
		OA0=1;
		ODAT=0;
		OCL1=1;
		OCL1=0;
		OCL1=1;
	}
}

void ReturnLam_be(void)
{
if	(oldhw)
		{
		OA2=0;
		OA1=1;
		OA0=0;
		ODAT=1;
		OCL1=1;
		OCL1=0;
		OCL1=1;
	}
}

void ReturnLam_ki(void)
{
if	(oldhw)
		{
		OA2=0;
		OA1=1;
		OA0=0;
		ODAT=0;
		OCL1=1;
		OCL1=0;
		OCL1=1;
	}
}



void Delayms (int dms)
{
int	del1, del2;

//di();
del1 = 0;
del2=0;
while(++del2 < dms)
	{
	del1 = 0;
while(++del1 < 49)
		{  // üres ciklus
		}
	}
//ei();
}



//******************************************************
// Hardver inicializáló rutin
//******************************************************

void	Init_HW(void)
{	
		pctap_ki();	//			; számítógép kikapcs.
		montap_ki();		//			; monitor kikapcs.
		amptap_ki();		//			; erősítő kikapcs.
		atxst_ki();			//			; számítógép indítás kikapcs.

		buzz_off();		//			; zümmer kikapcs
		ermtilt_be();		//		; érmevizsgáló tiltása
		mute_be();		//			; erősítő némítás bekapcs.
		counter_off();		//		; számláló kikapcs

		StepLeftLam_ki();		// régi vezérlő nyomogomb lampak kikapcs
		StepRightLam_ki();
		PageLeftLam_ki();
		PageRightLam_ki();
		EnterLam_ki();
		ReturnLam_ki();

		sleep_led_on();	//		; sleep LED bekapcs - jelzi a SW futását
		err_led_off();		//		; error LED ne égjen
		run_led_off();		//		; start LED kikapcs
		kibe_lam_off();		//		; ki-be lámpa kikapcs
		erme_lam_off();	//		; érme lámpa kikapcs

		vent_lassu();		//		; ventillátor lassan forogjon
		ron_ki();			//			; távadó kikapcs
		roff_ki();			//			; távadó kikapcs

} // init_hw



/*******************************************************************************************
*  Bemenetek olvasása
*
********************************************************************************************/

unsigned char KIBE_Pressed (void)
{

	c2 = 1;		// vegyük úgy, hogy meg van nyomva

if (oldhw == 1)
	{
	OA2 = 1;	
	OA1 = 1;	
	OA0 = 1; 
	if (OIN1) 
			{ 
			c2 = 0;
	 		}	// nullázzuk, ha még sincs megnyomva
	} else
	{
		if (b_kibe == 1)
		  {
	 	c2 = 0;
	 	}	// nullázzuk, ha még sincs megnyomva
	}
	
	return  c2;
}



unsigned char ERMEAJTO_Nyitva (void)
{

	c2 = 1;		// vegyük úgy, hogy meg van nyomva

if (oldhw == 1)
	{
	OA2 = 1;	
	OA1 = 1;	
	OA0 = 1; 
	if (OIN2) 
			{ 
			c2 = 0;
	 		}	// nullázzuk, ha még sincs megnyomva
	} else
	{
	A2 = 0;	
	A1 = 0;	
	A0 = 0; 
	if (b_vol) 
		  {
	 	c2 = 0;
	 	}	// nullázzuk, ha még sincs megnyomva
	}
	
	return  c2;
}


unsigned char SETUP_Pressed (void)
{

	c2 = 1;		// vegyük úgy, hogy meg van nyomva

if (oldhw == 1)
	{

#ifdef DEBUG  // DEBUG esetén RC0-t használjuk

	if (RB5)   // ez majd RB6 lesz

#else 

	if (RB6)   // nem debug módban RB6 lesz

#endif
			{ 
			c2 = 0;
	 		}	// nullázzuk, ha még sincs megnyomva
	} else
	{
	A2 = 0;	
	A1 = 0;	
	A0 = 1; 
		if (b_serv)
		  {
	 	c2 = 0;
	 	}	// nullázzuk, ha még sincs megnyomva
	}
	
	return  c2;
}



unsigned char TOUCH_Kezeles (void)
{

	c2 = 1;		// vegyük úgy, hogy touchscreen

if (oldhw)
	{
	OA2 = 0;	
	OA1 = 0;	
	OA0 = 0; 
	if (OIN1) 
			{ 
			c2 = 0;
	 		}	// nullázzuk, ha nem touch
	} else
	{

	A2 = 0;	
	A1 = 0;	
	A0 = 0; 
	if (b_dip) 
			{ 
			c2 = 0;
	 		}	// nullázzuk, ha nem touch

	}
	
	return  c2;
}


// DIP8 - =1, ha a monitort induláskor be kell kapcsolni
unsigned char DIP_Monitor (void)
{

	c2 = 1;		// vegyük úgy, hogy 

if (oldhw)
	{
		c2 = 0;		// régi gépnél nincs ilyen
	} else
	{
	A2 = 1;	
	A1 = 1;	
	A0 = 1; 
	if (b_dip) 
			{ 
			c2 = 0;
	 		}	// nullázzuk, ha OFF állapotban van a kapcsoló

	}
	
	return  c2;
}

// DIP7 - =1, ha hosszabb kikapcsolási időt kell hagyni
unsigned char DIP_Longoff (void)
{

	c2 = 1;		// vegyük úgy, hogy 

if (oldhw)
	{
		c2 = 0;		// régi gépnél nincs ilyen
	} else
	{
	A2 = 1;	
	A1 = 1;	
	A0 = 0; 
	if (b_dip) 
			{ 
			c2 = 0;
	 		}	// nullázzuk, ha OFF állapotban van a kapcsoló

	}
	return  c2;
}



// DIP3  =1, ha engedélyezzük billentyuzet használatát
unsigned char DIP_Keyboard (void)
{

	c2 = 'K';		// vegyük úgy, hogy engedélyezett 

if (oldhw)
	{
		c2 = 'N';		// régi gépnél nincs ilyen
	} else
	{
	A2 = 0;	
	A1 = 1;	
	A0 = 0; 
	if (b_dip) 
			{ 
			c2 = 'N';
	 		}	// nullázzuk, ha OFF állapotban van a kapcsoló

	}
	
	return  c2;
}

// DIP4  =1, ha kétcsatornás távvezérlő van
unsigned char DIP_2ch_Remote (void)
{

	c2 = 1;		// vegyük úgy, hogy 

if (oldhw)
	{				// régi gépnél nincs DIP-kapcsoló
		c2 = RA2;		// régi gépnél 2ch-s távvezérlőnél az RA2 bemenet lebeg, 
					// ezzel lehet megismerni, hogy csak kétcsatornás
					// Ha az RA2 H szint, akkor csak kétcsatornás
	} else
	{
	A2 = 0;	
	A1 = 1;	
	A0 = 1; 
	if (b_dip) 
			{ 
			c2 = 0;
	 		}	// nullázzuk, ha OFF állapotban van a kapcsoló

	}
	
	return  c2;
}

// DIP2  =1, ha mechanikus érmevizsgáló van a gépben
// régi gépen jumper
unsigned char MECH_Ermevizsgalo (void)
{

	c2 = 1;		// vegyük úgy, hogy meg van nyomva

if (oldhw)
	{
	OA2 = 0;	
	OA1 = 0;	
	OA0 = 0; 
	if (OIN2) 
			{ 
			c2 = 0;
	 		}	// nullázzuk, ha nincs jumper
	} else
	{

	A2 = 0;	
	A1 = 0;	
	A0 = 1; 
	if (b_dip) 
			{ 
			c2 = 0;
	 		}	// nullázzuk, nincs ON állásban

	}
	
	return  c2;
}



unsigned char PageLeft_Pressed (void)
{

	c2 = 1;		// vegyük úgy, hogy meg van nyomva

if (oldhw)
	{
	OA2 = 1;	
	OA1 = 1;	
	OA0 = 0; 
	if (OIN2) 
			{ 
			c2 = 0;
	 		}	// nullázzuk, ha még sincs megnyomva
	} else
	{

	A2 = 1;	
	A1 = 0;	
	A0 = 1; 
	if (!b_but) 
			{ 
			c2 = 0;
	 		}	// nullázzuk, ha még sincs megnyomva
	}
	return  c2;
}


unsigned char StepLeft_Pressed (void)
{

	c2 = 1;		// vegyük úgy, hogy meg van nyomva

if (oldhw)
	{
	OA2 = 1;	
	OA1 = 0;	
	OA0 = 1; 
	if (OIN2) 
			{ 
			c2 = 0;
	 		}	// nullázzuk, ha még sincs megnyomva
	} else
	{

	A2 = 1;	
	A1 = 1;	
	A0 = 0; 
	if (!b_but) 
			{ 
			c2 = 0;
	 		}	// nullázzuk, ha még sincs megnyomva
	}
	return  c2;
}


unsigned char Return_Pressed (void)
{

	c2 = 1;		// vegyük úgy, hogy meg van nyomva

if (oldhw)
	{
	OA2 = 1;	
	OA1 = 0;	
	OA0 = 0; 
	if (OIN2) 
			{ 
			c2 = 0;
	 		}	// nullázzuk, ha még sincs megnyomva
	} else
	{

	A2 = 1;	
	A1 = 1;	
	A0 = 1; 
	if (!b_but) 
			{ 
			c2 = 0;
	 		}	// nullázzuk, ha még sincs megnyomva
	}
	return  c2;
}


unsigned char Enter_Pressed (void)
{

	c2 = 1;		// vegyük úgy, hogy meg van nyomva

if (oldhw)
	{
	OA2 = 0;	
	OA1 = 1;	
	OA0 = 1; 
	if (OIN2) 
			{ 
			c2 = 0;
	 		}	// nullázzuk, ha még sincs megnyomva
	} else
	{

	A2 = 0;	
	A1 = 1;	
	A0 = 1; 
	if (!b_but) 
			{ 
			c2 = 0;
	 		}	// nullázzuk, ha még sincs megnyomva
	}
	return  c2;
}


unsigned char StepRight_Pressed (void)
{

	c2 = 1;		// vegyük úgy, hogy meg van nyomva

if (oldhw)
	{
	OA2 = 0;	
	OA1 = 1;	
	OA0 = 0; 
	if (OIN2) 
			{ 
			c2 = 0;
	 		}	// nullázzuk, ha még sincs megnyomva
	} else
	{

	A2 = 0;	
	A1 = 1;	
	A0 = 0; 
	if (!b_but) 
			{ 
			c2 = 0;
	 		}	// nullázzuk, ha még sincs megnyomva
	}
	return  c2;
}


unsigned char PageRight_Pressed (void)
{

	c2 = 1;		// vegyük úgy, hogy meg van nyomva

if (oldhw)
	{
	OA2 = 0;	
	OA1 = 0;	
	OA0 = 1; 
	if (OIN2) 
			{ 
			c2 = 0;
	 		}	// nullázzuk, ha még sincs megnyomva
	} else
	{

	A2 = 1;	
	A1 = 0;	
	A0 = 0; 
	if (!b_but) 
			{ 
			c2 = 0;
	 		}	// nullázzuk, ha még sincs megnyomva
	}
	return  c2;
}


unsigned char CH1_Coin (void)
{

	c2 = 1;		// vegyük úgy, hogy meg van nyomva

if (oldhw)
	{
	OA2 = 0;	
	OA1 = 1;	
	OA0 = 1; 
	if (OIN1) 
			{ 
			c2 = 0;
	 		}	// nullázzuk, ha még sincs megnyomva
	} else
	{

	A2 = 1;	
	A1 = 1;	
	A0 = 0; 
	if (b_erm) 
			{ 
			c2 = 0;
	 		}	// nullázzuk, ha még sincs megnyomva
	}
	return  c2;
}


unsigned char CH2_Coin (void)
{

	c2 = 1;		// vegyük úgy, hogy meg van nyomva

if (oldhw)
	{
	OA2 = 1;	
	OA1 = 0;	
	OA0 = 0; 
	if (OIN1) 
			{ 
			c2 = 0;
	 		}	// nullázzuk, ha még sincs megnyomva
	} else
	{

	A2 = 0;	
	A1 = 1;	
	A0 = 0; 
	if (b_erm) 
			{ 
			c2 = 0;
	 		}	// nullázzuk, ha még sincs megnyomva
	}
	return  c2;
}

unsigned char CH3_Coin (void)
{

	c2 = 1;		// vegyük úgy, hogy meg van nyomva

if (oldhw)
	{
	OA2 = 1;	
	OA1 = 1;	
	OA0 = 0; 
	if (OIN1) 
			{ 
			c2 = 0;
	 		}	// nullázzuk, ha még sincs megnyomva
	} else
	{

	A2 = 1;	
	A1 = 0;	
	A0 = 0; 
	if (b_erm) 
			{ 
			c2 = 0;
	 		}	// nullázzuk, ha még sincs megnyomva
	}
	return  c2;
}

unsigned char CH4_Coin (void)
{

	c2 = 1;		// vegyük úgy, hogy meg van nyomva

if (oldhw)
	{
	OA2 = 1;	
	OA1 = 0;	
	OA0 = 1; 
	if (OIN1) 
			{ 
			c2 = 0;
	 		}	// nullázzuk, ha még sincs megnyomva
	} else
	{

	A2 = 0;	
	A1 = 0;	
	A0 = 0; 
	if (b_erm) 
			{ 
			c2 = 0;
	 		}	// nullázzuk, ha még sincs megnyomva
	}
	return  c2;
}

unsigned char CH5_Coin (void)
{

	c2 = 1;		// vegyük úgy, hogy meg van nyomva

if (oldhw)
	{
	OA2 = 0;	
	OA1 = 0;	
	OA0 = 1; 
	if (OIN1) 
			{ 
			c2 = 0;
	 		}	// nullázzuk, ha még sincs megnyomva
	} else
	{

	A2 = 1;	
	A1 = 0;	
	A0 = 1; 
	if (b_erm) 
			{ 
			c2 = 0;
	 		}	// nullázzuk, ha még sincs megnyomva
	}
	return  c2;
}

unsigned char CH6_Coin (void)
{

	c2 = 1;		// vegyük úgy, hogy meg van nyomva

if (oldhw)
	{
	OA2 = 0;	
	OA1 = 1;	
	OA0 = 0; 
	if (OIN1) 
			{ 
			c2 = 0;
	 		}	// nullázzuk, ha még sincs megnyomva
	} else
	{

	A2 = 0;	
	A1 = 0;	
	A0 = 1; 
	if (b_erm) 
			{ 
			c2 = 0;
	 		}	// nullázzuk, ha még sincs megnyomva
	}
	return  c2;
}

/*********************************************************************************
* Távvezérlő bemenetek
*
*******************************************************************************/

unsigned char Remote_Mute (void)
{
	c2 = 0;		// vegyük úgy, hogy nincs megnyomva, távvezérlő H szinttel ad

if(SFbits.Remote_2ch)
	{
	return c2;		// 2 ch távvezérlőben nincs mute
	} else
	{  // 4ch távvezérlő
	 if (oldhw)	{if (RA5)   {c2 = 1;} } else	{if (RB4)	{ c2 = 1;	}}  // TV0
	return  c2;
	} 
} // Remote_Mute

/*
unsigned char Remote_TV0 (void)
{

	c2 = 0;		// vegyük úgy, hogy meg van nyomva

if (oldhw)	{if (RA2)   {c2 = 1;} } else	{if (RB4)	{ c2 = 1;	}}  // TV0
	return  c2;
}

*/

unsigned char Remote_Volup (void)
{
	c2 = 0;		// vegyük úgy, hogy nincs megnyomva, távvezérlő H szinttel ad

if(SFbits.Remote_2ch)
	{
	if (oldhw)	{if (RA5)   {c2 = 1;} } else	{if (RB4)	{ c2 = 1;	}}  // 
	return c2;		// 2 ch távvezérlőben a TV0 jel
	} else
	{  // 4ch távvezérlő

#ifdef DEBUG  // DEBUG esetén RB7, RB6 foglalt
	if (oldhw)	{if (RA4)   // ez majd RB7 lesz , ha nem debug
#else 
	if (oldhw)	{if (RB7)   // nem debug 
#endif
 	 {c2 = 1;} } else	{if (RB3)   { c2 = 1;}}		// T
	return  c2;

	} 
} // Remote_Volup


/*
unsigned char Remote_TV1 (void)
{

	c2 = 0;		// vegyük úgy, hogy meg van nyomva

if (oldhw)	{if (RA5)   {c2 = 1;} } else	{if (RB3)   { c2 = 1;}}		// TV1
	return  c2;
}
*/

unsigned char Remote_Voldn (void)
{
	c2 = 0;		// vegyük úgy, hogy nincs megnyomva, távvezérlő H szinttel ad

if(SFbits.Remote_2ch)
	{

#ifdef DEBUG  // DEBUG esetén RB7, RB6 foglalt
	if (oldhw)	{if (RA4)   // ez majd RB7 lesz , ha nem debug
#else 
	if (oldhw)	{if (RB7)   // nem debug 
#endif

   {c2 = 1;} } else	{if (RB3)   { c2 = 1;}}		// TV1
	return c2;		// 2 ch távvezérlőben a TV1 jel
	} else
	{  // 4ch távvezérlő
		if (oldhw)	
			{
			if (RA2) 	{ c2 = 1;	}
			} else 
			{
			if (RB2) { c2 = 1;	}
			}			//
	return  c2;
	} 
} // Remote_Voldn



/*
unsigned char Remote_TV2 (void)
{

	c2 = 0;		// vegyük úgy, hogy meg van nyomva

if (oldhw)	{if (RA4)   // ez majd RB7 lesz 
	{ c2 = 1;	} } else {if (RB2) { c2 = 1;	}}			//TV2
	return  c2;
}
*/

unsigned char Remote_Vol4b (void)		// 4. távvezérlő gomb
{
	c2 = 0;		// vegyük úgy, hogy nincs megnyomva, távvezérlő H szinttel ad

if(SFbits.Remote_2ch)
	{
	return c2;		// 2 ch távvezérlőben nincs 4. gomb
	} else
	{  // 4ch távvezérlő
if (oldhw) {c2 = 0;		// régi hw-en nincs, sose lesz megnyomva	
	} else {A2 = 1;	A1 = 1;	A0 = 1; 	if (b_erm) { c2 = 1;}	}  // TV3
	return  c2;
	} 
} // Remote_Mute

/*
unsigned char Remote_TV3 (void)
{

	c2 = 0;		// vegyük úgy, hogy meg van nyomva

if (oldhw) {c2 = 0;		// régi hw-en nincs, sose lesz megnyomva	
	} else {A2 = 1;	A1 = 1;	A0 = 1; 	if (b_erm) { c2 = 1;}	}
	// a távvezérlő magas szintet ad megnyomáskor
	return  c2;
}
*/

//void ser_put(unsigned char c)
//{
	/* output one byte */
//	while(!TXIF)	/* set when register is empty */
//		continue;
//	TXREG = c;

//	while (((txiptr+1) & SER_FIFO_MASK)==txoptr)
//		{
//		s1 = s2;
//		}		
	// continue;
	
//	GIE=0;		// tiltunk minden megszakitást
//	txfifo[txiptr] = c;		// karaktert bemásoljuk a küldő pufferbe
//	txiptr=(txiptr+1) & SER_FIFO_MASK;	// pointer növelés, körbefordulás kezelés
//	TXIE=1;		// engedélyezzük az adást
//	GIE=1;		// engedélyezzük a megszakításokat
// }

void	putch(unsigned char byte) 
{
	di();
	/* output one byte */
	while(!TXIF)	/* set when register is empty */
		continue;
	TXREG = byte;
//	txfifo[txiptr] = byte;				// betöltjük a kimenő regiszterbe
//	txiptr++;
//	txiptr &= SER_FIFO_MASK;		// kezeljük a körbefordulást

//	TXIE=1;						// engedélyezzük az adást
	ei();	// teszt
}

/*************************************************************
   Hangjelzést adó rutinok
**************************************************************/

void		beep(BP_TYPE beep_type)
{
unsigned char	reg1;

	switch(beep_type)
		{
	case ok: 	{ 
			buzz_on();
			Delayms(60);
			buzz_off();	
			}
			break;

	case serv:	{ 
			buzz_on();
			Delayms(80);
			buzz_off();
			Delayms(40);
			buzz_on();
			Delayms(40);
			buzz_off();
			}
			break;

	case error: 	{ 
			reg1 = 0;
			while (reg1++ < 4)
				{
				buzz_on();
				Delayms(80);
				buzz_off();
				Delayms(80);
				} // while
			}
			break;
	case off: 		{ 
			reg1 = 0;
			while (reg1++ < 2)
				{
				buzz_on();
				Delayms(80);
				buzz_off();
				Delayms(80);
				} // while
			}
			break;
	} // case
} // beep rutin
	

// három gyor ATX impulzus egymás után - a régebbi gépek nem indultak el egy impulzustól
void		atxst_pulse (void)
	{
			atxst_be();
			Delayms(200);
			atxst_ki();
			Delayms(200);
			atxst_be();
			Delayms(200);
			atxst_ki();
			Delayms(200);
			atxst_be();
			Delayms(200);
			atxst_ki();
			Delayms(200);
			
}


void		send_mess (unsigned char str)
{
//	bor();  ////BOR figyeles
	putch(STX);
	putch(str);
	putch(ETX);
//	putch(0x00);
}



/*************************************************
	RX üzenetszámláló csökkentése
*************************************************/
void		Dec_RX_Msg(void)
{
	di();					// megszakitás tiltva
	if(RX_Msg)				// ha nem nulla, csökkentjük
		{
		RX_Msg--;
		} 

	if(!RX_Msg && !SFbits.In_Msg)		// ha nulla lett az üzenetek száma, és pont nincs bejövő üzenet, nulla legyen
		{							// EEPROM írásos rutin alatt (pl. 100 ms) jöhet be valami, azt ne nyeljük be
		rxoptr = rxiptr;			// a két pointer egyezzen meg, nincs feldolgozatlan üzenet
		}
	ei();					// megszakitás emngedélyezve
}

	
/********************* ide ugrik, ha megnyomtak a service-mod gombot
	setup nyomogomb vizsgalata
*******************************************************************/

void		serv_mode (void)
{

}

void	Send_HW_Init(void)
{
unsigned char	local_CLC;

	putch(STX);
	local_CLC = 0;
	putch(m_startup);
	local_CLC++;

/* EEPROM-ból ment a service kód
		for(s1=8; s1<16; s1++)
		{
		Char_To_Send = EEPROM_READ(s1);
		putch(Char_To_Send);
		local_CLC++;
		}
*/

		for(s1=0; s1<13; s1++)
		{
		Char_To_Send = scode[s1];
		putch(Char_To_Send);
		local_CLC++;
		}


//	for(s1=1; s1<6; s1++)
//		{
//		Char_To_Send = EEPROM_READ(s1);
//		putch(Char_To_Send);
//		local_CLC++;
//		}

	s1 = TOUCH_Kezeles();
	if (s1==1)
		{
		Char_To_Send =  'T';		// ha fel van kapcsolva a dip 1 = touch
		} else
		{
		Char_To_Send =  'B';
		} 
		putch(Char_To_Send);
		local_CLC++;

	s1 =  MECH_Ermevizsgalo();
	if (s1==1)
		{
		Char_To_Send =  'M';
		} else
		{
		Char_To_Send =  'E';
		} 
		putch(Char_To_Send);
		local_CLC++;

	if (SFbits.Clear_Credit)
		{
		Char_To_Send =  'C';
		} else
		{
		Char_To_Send =  'N';
		} 
		putch(Char_To_Send);
		local_CLC++;

	if (SFbits.Keyboard_Enable)
		{
		Char_To_Send =  'Y';
		} else
		{
		Char_To_Send =  'N';
		} 
		putch(Char_To_Send);
		local_CLC++;

// #ifdef DEBUG  // DEBUG esetén RB7, RB6 foglalt

		BadButBits.t2ch = SFbits.Remote_2ch;		// küldjük azt is, hogy 2 csatornás a ávvezérlő

// #endif 
		Char_To_Send =  BadButBits.x;			//	elküldjük a beragadt gombok azonositoit
		putch(Char_To_Send);
		local_CLC++;

		local_CLC = ~local_CLC + 1;
		putch(local_CLC);
		putch(ETX);
	
}


/*********************************************************************
* Szám formátum átalakítása ASCII kódolású stringre
*********************************************************************/
int Num2Dec (DWORD BeNum)	
{
// char ctmp[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'}
DWORD	maradek;
unsigned char tpo;

	di();						// megszakitás tiltása
	s1 = 0;					// a kimeno stringet kinullázzuk
	while ( ++s1 < 10) 
		{
		Dec_str[s1] = 0;		// nem ASCII nulla! Nem értékes számjegyek
		}
	Dec_str[0] = '0';			// Ha nulla lenne a szám

	s1 = 0;
	while (BeNum > 0)
		{
		maradek = BeNum % 10;		// (%) operator - maradékos osztás maradéka
		Dec_str[s1] = ctmp[maradek];
		BeNum = BeNum / 10;		// osztás törtek elhagyásával
		s1++;
		}
	ei();				// megszakitás engedélyezése
}


/*****************************************************************
* dátum be- és kikodolása (tömöritése, kicsomagolasa)
* bejövő adat: 6 db ASCII karakter: EE-HH-NN a Date_Str tömbben
* kimenő adat: 1 Integer
* képlet: EV * 550 + HO * 40 + NAP
*****************************************************************/
/*
unsigned int 	Date_Encode(void)
{
unsigned int kidate;

	kidate =  550 * ( (DateStr[0] - '0') * 10 + (DateStr[1] - '0'));
	kidate = kidate + 40 * ( (DateStr[2] - '0') * 10 + (DateStr[3] - '0'));
	kidate = kidate + (DateStr[4] - '0') * 10 + (DateStr[5] - '0');
	return kidate;
}


*/

void 	Date_Decode(unsigned int DateZipped)
{
unsigned char Ev, Ho, Nap;

	di();						// megszakitas letiltva
	Ev = DateZipped / 550;
	DateStr[0] = '0' + (Ev / 10);
	DateStr[1] = '0' + (Ev % 10);

	Ho = (DateZipped % 550) / 40;
	DateStr[2] = '0' + (Ho / 10);
	DateStr[3] = '0' + (Ho % 10);

	Nap = DateZipped - 550 * Ev - 40 * Ho;
	DateStr[4] = '0' + (Nap / 10);
	DateStr[5] = '0' + (Nap % 10);
	ei();							// megszakitas ujra engedelyezve

//	 ((DateStr[0] - '0') * 10 + (DateStr[1] - '0') * 500) +
//	((DateStr[2] - '0') * 10 + (DateStr[3] - '0') * 40) + 
//	(DateStr[4] - '0') * 10 + (DateStr[5] - '0');
//	return kidate;
}


// EEPROM írás ellenőrizve
// az írásnak és a visszaolvasásnak meg kell egyeznie
// Visszatérő érték az olvasások száma
unsigned char		CHK_eeprom_write(unsigned char addr, unsigned char value)
{
unsigned char Chk, RDVal;


		Wrts = 0;							// visszatérési hibajelzo paraméter

//		bor();    //////BOR figyeles
		if(eeprom_irhato)					// Wrts = 0, ha nincs hiba, Wrts = 1..5, ha több írási kiserlet volt, Wrts = 10, ha kobor irasi kiserlet volt
			{
			do
				{
				RDVal = value;			// későbbi ellenőrzőshez
				eeprom_write(addr, value);	// Kiiratás, megvárjuk a végét (gyári függvény)
				Chk = eeprom_read(addr);	// ellenőrző visszaolvasás
				if (Chk == value) break;		// Ha jó a visszaolvasás, kiugrunk a ciklusból;
				Wrts++;					// Wrts számolja, hányszor próbáltunk irni
				}
			while  (Wrts <5);
			}
			else	
				{
				Wrts = 9;
				}
			EEADR = 0xff;
			EEDATA = 0xff;
		return	Wrts;				// Ha wrts = 5, hibás az írás
		
}

// EEPROM olvasás ellenőrizve
// két egymást követő olvasásnak meg kell egyeznie
unsigned char		CHK_eeprom_read(unsigned char addr)
{
unsigned char Val1, Val2;

		do
			{
			Val1 = eeprom_read(addr);	// Beolvasás
			Val2 = eeprom_read(addr);	// ellenőrző beolvasás
			}
		while  (Val1 != Val2);			// ismétli, amig nem kap egyforma adatot
		return	Val1;				//  visszetérési érték az olvasott byte
		
}


/************************************************************************
* könyvelési adatok  írása az EEPROM-ba
* bejövő adatok:	adat sorszáma: bnum
* 				dátum a DateStr változóban EEHHNN formában
*				számláló állások a Booking_Counter változóban
*************************************************************************/


/************************************************************************
* számláló állások küldése ASCII formátumban
************************************************************************/

/*void		Chip_First_Run(void)
{
// először fut a chip

	// lekérdezzük a PC-től a setup-ot és aszámlálókat
	eeprom_write(0, 0x55);		// EEPROM-ba írjuk, hogy inicializáltuk a chipet
	s1 = eeprom_read(0);

}

*/
void	kamufuggveny(void);		//	Uses USART to receive data from PC

void	DownloadCode(void);		//	Uses USART to receive data from PC


/*****************************************************************
* érmeSETUP adatok beolvasása az EEPROM-ból
* 
******************************************************************/
void	EEPROM_CH_Read(void)
{
unsigned char local_addr, Val;

	for(s1= 0; s1< 6; s1++)
		{
		local_addr = s1 + setup_addr;
		Val = CHK_eeprom_read(local_addr);			// az érmecsatorna csak egy sorszámot tartalmaz
		Val -= '0';									// ASCII-ból legyen szám
		CH_Money[s1+1] = Money_Val[Val];
		}
}

/*****************************************************************
* Counters adatok beolvasása az EEPROM-ból
* 
******************************************************************/
void	EEPROM_Counters_Read(void)
{
unsigned char local1_addr, local2_addr, local3_addr;

	for(s1= 0; s1< 8; s1++)
		{
		local1_addr = s1 + counter1_addr;
		Counter.cnumbyte[s1] = EEPROM_READ(local1_addr);   // a nagybetűs EEPROM_READ makró, nem függvény
		local2_addr = s1 + counter2_addr;
		Counter2.cnumbyte[s1] = EEPROM_READ(local2_addr);
		local3_addr = s1 + counter3_addr;
		Counter3.cnumbyte[s1] = EEPROM_READ(local3_addr);
//		Setup_Tomb[s1] = EEPROM_READ((s1+setup_addr));
		}
	if ((Counter.Hosszu != Counter2.Hosszu) || (Counter.Rovid != Counter2.Rovid) 		// ez a feltétel akkor igaz, ha a counter sem a counter2-vel,
		&& (Counter.Hosszu != Counter3.Hosszu) || (Counter.Rovid != Counter3.Rovid))	// sem a counter 3-mal nem egyezik meg: mert ha valamelyikkel
																		// megegyezik, akkor a counterben jó érték van. (2 egyenlő a háromból)
		{
		if ((Counter2.Hosszu == Counter3.Hosszu) && (Counter2.Rovid == Counter3.Rovid))	// ha a counter 2 és 3 teljesen megegyezik, ez a jó érték
			{
			Counter.Hosszu = Counter2.Hosszu;
			Counter.Rovid = Counter2.Rovid;
			}
		}

}

/*****************************************************************
* Counters adatok írása az EEPROM-ba.
* Minden érme bedobás után írunk
* 
******************************************************************/
void	EEPROM_Counters_Write(void)
{
unsigned char local1_addr, local2_addr, local3_addr;

				eeprom_irhato = 1;				// engedélyezzük aqz eeprom írást
				for(s1= 0; s1< 8; s1++)			// számlálók aaktuális értékének beolvasása
					{
					local1_addr = s1 + counter1_addr;
					Chk = CHK_eeprom_write(local1_addr, Counter.cnumbyte[s1]);    // EEPROM-ba írjuk
					local2_addr = s1 + counter2_addr;
					Chk = CHK_eeprom_write(local2_addr, Counter.cnumbyte[s1]);    // mind a három területre ugyanazt írjuk
					local3_addr = s1 + counter3_addr;
					Chk = CHK_eeprom_write(local3_addr, Counter.cnumbyte[s1]);    // 
					}
				eeprom_irhato = 0;				// tiltjuk az eeprom írást

}


/*****************************************************************
* soros vonalon érkezett üzeneteket kezeli
* akkor ugrik ide a program, ha kész üzenet van a pufferben
******************************************************************/

// zárási adatrekord a PC felől
void	Get_Booking(void)
{
		//		rxoptr += 2;					// STX és parancskód átlépése
		//		rxoptr &=	SER_FIFO_MASK;		// körbefordulás kezelése
				DateStr[0] = rxfifo[rxoptr];		// a puffer harmadik karakterétől jön 6 karakter dátum
				rxoptr ++;						//  pointer növelés
				rxoptr &=	SER_FIFO_MASK;		// körbefordulás kezelése
				DateStr[1] = rxfifo[rxoptr];		// a puffer második karakterétől jön 6 karakter dátum
				rxoptr ++;						//  pointer növelés
				rxoptr &=	SER_FIFO_MASK;		// körbefordulás kezelése
				DateStr[2] = rxfifo[rxoptr];		// a puffer második karakterétől jön 6 karakter dátum
				rxoptr ++;						//  pointer növelés
				rxoptr &=	SER_FIFO_MASK;		// körbefordulás kezelése
				DateStr[3] = rxfifo[rxoptr];		// a puffer második karakterétől jön 6 karakter dátum
				rxoptr ++;						//  pointer növelés
				rxoptr &=	SER_FIFO_MASK;		// körbefordulás kezelése
				DateStr[4] = rxfifo[rxoptr];		// a puffer második karakterétől jön 6 karakter dátum
				rxoptr ++;						//  pointer növelés
				rxoptr &=	SER_FIFO_MASK;		// körbefordulás kezelése
				DateStr[5] = rxfifo[rxoptr];		// a puffer második karakterétől jön 6 karakter dátum
				rxoptr ++;						// pointer növelés
				rxoptr &=	SER_FIFO_MASK;		// körbefordulás kezelése

				Date_Store.Zipped =  550 * ( (DateStr[0] - '0') * 10 + (DateStr[1] - '0'));
				Date_Store.Zipped = Date_Store.Zipped + 40 * ( (DateStr[2] - '0') * 10 + (DateStr[3] - '0'));
				Date_Store.Zipped = Date_Store.Zipped + (DateStr[4] - '0') * 10 + (DateStr[5] - '0');
				// kapott dátum betömörítése

		
				for(s1= 0; s1< 8; s1++)			// számlálók aaktuális értékének beolvasása
					{
					AddrL = s1 + counter1_addr;
					Counter.cnumbyte[s1] = CHK_eeprom_read(AddrL);
					}

				AddrL = CHK_eeprom_read(BKPointer);    // beolvassuk a következő zárási cimet
	//			bnum = (bnum * 8) + booking_addr;	// A könyvelés terület kezdetére ugrunk
				RH = Counter.R2;				// a rövid számláló MSB bitjeit kezeljük
				RH &= 0x0F;						// az alsó tetrád marad 
				RH <<= 4;
				Counter.H3 &= 0x0F;				// itt is csak az alsó 4 bit kell
				RH |= Counter.H3;				// 

				eeprom_irhato = 1;
				Chk = CHK_eeprom_write(AddrL,Date_Store.DLSB);    // byte-onként beolvassuk a zárási adatot
				AddrL++;
				Chk = CHK_eeprom_write(AddrL, Date_Store.DMSB);    // byte-onként beolvassuk a zárási adatot
				AddrL++;
				Chk = CHK_eeprom_write(AddrL, RH);    // byte-onként beolvassuk a zárási adatot
				AddrL++;
				Chk = CHK_eeprom_write(AddrL, Counter.H2);    // byte-onként beolvassuk a zárási adatot
				AddrL++;
				Chk = CHK_eeprom_write(AddrL, Counter.H1);    // byte-onként beolvassuk a zárási adatot
				AddrL++;
				Chk = CHK_eeprom_write(AddrL, Counter.H0);    // byte-onként beolvassuk a zárási adatot
				AddrL++;
				Chk = CHK_eeprom_write(AddrL, Counter.R1);    // byte-onként beolvassuk a zárási adatot
				AddrL++;
				Chk = CHK_eeprom_write(AddrL, Counter.R0);    // byte-onként beolvassuk a zárási adatot
				AddrL++;
				if (AddrL == 240) { AddrL = 48; }				// kezeljük a pointer körbefordulását
				Chk = CHK_eeprom_write(BKPointer, AddrL);    // EEPROM-ba a következő zárási pointer

				Counter.Rovid = 0;					// nullázás
				
				for(s1= 0; s1< 8; s1++)			// számlálók aaktuális értékének beolvasása
					{
					AddrL = s1 + counter1_addr;
					Chk = CHK_eeprom_write(AddrL, Counter.cnumbyte[s1]);    // EEPROM-ba írjuk
					AddrL = s1 + counter2_addr;													//////////////
					Chk = CHK_eeprom_write(AddrL, Counter.cnumbyte[s1]);    // EEPROM-ba írjuk			//////////////
					AddrL = s1 + counter3_addr;													//////////////
					Chk = CHK_eeprom_write(AddrL, Counter.cnumbyte[s1]);    // EEPROM-ba írjuk			//////////////
					}

				eeprom_irhato = 0;

				rxoptr++;						// ETX átlépése
				rxoptr &=	SER_FIFO_MASK;		// körbefordulás kezelése
				RX_Msg --;					// egy üzenet feldolgozva
		
				putch(STX);		// nyugta
				putch(ACK);
				putch(ETX);

} // end of Get_Booking


void	Wait_For_Ack(void)
{
	while(!RX_Msg) 
		{
		}		// v�r egy bejövő üzenetet
		di();
		rxoptr++;		// ha j�tt valami, az STX-et �tl�pj�k
		rxoptr++;
		rxoptr++;
		rxoptr &= SER_FIFO_MASK;
		Dec_RX_Msg();				// üzenet kezelve
		ei();
	
}



void Send_Booking(void)
{
			s2 = CHK_eeprom_read(BKPointer);    // beolvassuk a következő zárási cimet
			mpl = 0; 							// 24-szer csin�ljuk meg a ciklust
			do
				{
				if (s2 == booking_addr)				// kiszámitjuk az utolsó zárás cim�t
					{
					s2 = last_booking_addr;			// a 8-cal kor�bbi cimen van a legfrissebb zárás
					} else						// körbefordulás kezelés
					{
					s2 = s2 - 8;
					}
				
			//	putch(12);				// LineFeed
			//	putch(15);				// Carriage Return
				AddrL = s2;
	//		AddrL = (bnum * 8) + booking_addr;	// A könyvelés terület kezdet�re ugrunk
				Date_Store.DLSB = CHK_eeprom_read(AddrL);
				AddrL++;
				Date_Store.DMSB = CHK_eeprom_read(AddrL); 
				AddrL++;
				RH = CHK_eeprom_read(AddrL);    // byte-onként beolvassuk a zárási adatot
				AddrL++;
				Booking_Counter.H2 = CHK_eeprom_read(AddrL);    // byte-onként beolvassuk a zárási adatot
				AddrL++;
				Booking_Counter.H1 = CHK_eeprom_read(AddrL);    // byte-onként beolvassuk a zárási adatot
				AddrL++;
				Booking_Counter.H0 = CHK_eeprom_read(AddrL);    // byte-onként beolvassuk a zárási adatot
				AddrL++;
				Booking_Counter.R1 = CHK_eeprom_read(AddrL);    // byte-onként beolvassuk a zárási adatot
				AddrL++;
				Booking_Counter.R0 = CHK_eeprom_read(AddrL);    // byte-onként beolvassuk a zárási adatot

				Date_Decode(Date_Store.Zipped);					// kapott dátum betömör�t�se
				Booking_Counter.H3 = RH;					// 
				Booking_Counter.H3 &= 0x0F;				// Hosszu számláló első 4 bitje 

				Booking_Counter.R2 = RH;						// a rövid számláló első 4 bitje lesz
				Booking_Counter.R2 &= 0xF0;					// az RH felső tetr�dja 
				Booking_Counter.R2 >>= 4;

				putch(STX);				// STX
				local_CLC = 0;
				putch(m_booking_send);		// parancskód
				local_CLC++;

				Char_To_Send = 'A' + mpl;
				putch(Char_To_Send);				// zárás sorszáma + 'A'
				local_CLC++;

				for(s3=0; s3 < 6; s3++)
					{
					Char_To_Send = DateStr[s3];		// A DateStr byte-jait egyes�vel kik�ldj�k
					putch(Char_To_Send);
					local_CLC++;
					}

				Char_To_Send = m_space;		// Space karakter elv�laszt�nak
				putch(Char_To_Send);
				local_CLC++;

				Num2Dec(Booking_Counter.Hosszu);	// hosszu számláló �talak�t�sa ASCII stringg�
				dcmut = 9;
				while(Dec_str[dcmut] == 0) 	// Felesleges bal oldali null�k átlépése
					{
					dcmut--;
					}

				while(dcmut >= 0)	// t�lcsordul�sig megy�nk
					{
					Char_To_Send = Dec_str[dcmut];
					putch(Char_To_Send);
					local_CLC++;
 					dcmut--;
					}
	
				Char_To_Send = m_space;		// Space karakter elv�laszt�nak
				putch(Char_To_Send);
				local_CLC++;
	
				Num2Dec(Booking_Counter.Rovid);	// rövid számláló �talak�t�sa ASCII stringg�
				dcmut = 9;
				while(Dec_str[dcmut] == 0) 	// Felesleges bal oldali null�k átlépése
					{
					dcmut--;
					}

				while(dcmut >= 0)	// t�lcsordul�sig megy�nk
					{
					Char_To_Send = Dec_str[dcmut];
					putch(Char_To_Send);
					local_CLC++;
 					dcmut--;
					}
	
				Char_To_Send = m_space;		// Space karakter elv�laszt�nak
				putch(Char_To_Send);
				local_CLC++;

				local_CLC = ~local_CLC + 1;
				putch(local_CLC);
				putch(ETX);
				Wait_For_Ack();			// megvárjuk a nyugt�t
				Delayms(10);				// késleltetés
				mpl++;					// pozitiv nyugta j�tt, ciklusszámláló növelés
		
		} while (mpl < 24);	// end of do ciklus
	} // end of send_booking



// számláló állások a PC fel�l
// EEPROM-ba �rni
void	Get_Counters(void)
{
unsigned char AddrL2, AddrL3;

				while (rxfifo[rxoptr] != ETX)	
					{
					 rxoptr++;
					 rxoptr &= SER_FIFO_MASK;	
					}		 // megkeress�k az üzenet v�g�t
				rxoptr++;				
				 rxoptr &= SER_FIFO_MASK;		// az ETX_et is �tl�pj�k
				s1 = rxoptr;
				if(s1 == 0) { s1 = SER_FIFO_MASK; } else { s1--;}	// s1-et cs�kkentj�k
				if(s1 == 0) { s1 = SER_FIFO_MASK; } else { s1--;}	// s1-et cs�kkentj�k
				if(s1 == 0) { s1 = SER_FIFO_MASK; } else { s1--;}	// s1-et cs�kkentj�k
				if(s1 == 0) { s1 = SER_FIFO_MASK; } else { s1--;}	// s1-et cs�kkentj�k		// összesen 4-el kell cs�kkenteni
	
//			s1 = rxoptr - 4; 		// visszafel� elemezzük az üzenetet. A -4 pozicio a rovid számláló v�ge
				dcmut = 0;
				while(rxfifo[s1] != ' ') 	// amig nem space, m�solunk
					{
					Dec_str[dcmut] = rxfifo[s1] - '0';
					dcmut++;						// dcmut-ot n�velni
					if(s1 == 0) { s1 = SER_FIFO_MASK; } else { s1--;}	// s1-et cs�kkentj�k
												// rxfifo-ban visszafel� megy�nk
					}

				Counter.Rovid = 0;
				mpl = 1;						// aaktuális szorzo értéke
				for(s2=0; s2<dcmut; s2++)
					{
					Counter.Rovid = Counter.Rovid + (mpl * Dec_str[s2]);
					mpl = 10 * mpl;			// aaktuális szorzo növelése
					}

					if(s1 == 0) { s1 = SER_FIFO_MASK; } else { s1--;}	// s1-et cs�kkentj�k
		//		s1--; 		// �tugorjuk az elv�laszt�t, feldolgozzuk a hosszu számlálót
				dcmut = 0;
				while(rxfifo[s1] != ' ') 	// amig nem space, m�solunk
					{
					Dec_str[dcmut] = rxfifo[s1] - '0';
					dcmut++;						// dcmut-ot n�velni
					if(s1 == 0) { s1 = SER_FIFO_MASK; } else { s1--;}	// s1-et cs�kkentj�k
		//			s1--;							// rxfifo-ban visszafel� megy�nk
					}

				Counter.Hosszu = 0;
				mpl = 1;						// aaktuális szorzo értéke
				for(s2=0; s2<dcmut; s2++)
					{
					Counter.Hosszu = Counter.Hosszu + (mpl * Dec_str[s2]);
					mpl = 10 * mpl;			// aaktuális szorzo növelése
					}


	//			AddrL = CHK_eeprom_read(BKPointer);    // beolvassuk a következő zárási cimet
	//			bnum = (bnum * 8) + booking_addr;	// A könyvelés terület kezdet�re ugrunk


				eeprom_irhato = 1;
				for(s1= 0; s1< 8; s1++)
					{
					AddrL = s1 + counter1_addr;
					Chk = CHK_eeprom_write(AddrL, Counter.cnumbyte[s1]);    // byte-onként ki�rjuk a számlálókat
					AddrL2 = s1 + counter2_addr;
					Chk = CHK_eeprom_write(AddrL2, Counter.cnumbyte[s1]);    // ugyanazt �rjuk mindenhova
					AddrL3 = s1 + counter3_addr;
					Chk = CHK_eeprom_write(AddrL3, Counter.cnumbyte[s1]);    // ugyanazt �rjuk mindenhova
					}
				eeprom_irhato = 0;

				Dec_RX_Msg();		// üzenet lekezelve
				putch(STX);		// nyugta
				putch(ACK);
				putch(ETX);

} // end of Get_Counters


void Send_Counters(void)
{
unsigned char local2_addr, local3_addr;

			for(s1= 0; s1< 8; s1++)
				{
				AddrL = s1 + counter1_addr;
				Counter.cnumbyte[s1] = CHK_eeprom_read(AddrL); 
				local2_addr = s1 + counter2_addr;
				Counter2.cnumbyte[s1] = CHK_eeprom_read(local2_addr);
				local3_addr = s1 + counter3_addr;
				Counter3.cnumbyte[s1] = CHK_eeprom_read(local3_addr);
				}

	if ((Counter.Hosszu != Counter2.Hosszu) || (Counter.Rovid != Counter2.Rovid) 		// ez a feltétel akkor igaz, ha a counter sem a counter2-vel,
		&& (Counter.Hosszu != Counter3.Hosszu) || (Counter.Rovid != Counter3.Rovid))	// sem a counter 3-mal nem egyezik meg: mert ha valamelyikkel
																		// megegyezik, akkor a counterben j� érték van. (2 egyenlő a háromból)
		{
		if ((Counter2.Hosszu == Counter3.Hosszu) && (Counter2.Rovid == Counter3.Rovid))	// ha a counter 2 és 3 teljesen megegyezik, ez a j� érték
			{
			Counter.Hosszu = Counter2.Hosszu;
			Counter.Rovid = Counter2.Rovid;
			}
		}

			putch(STX);				// STX
			local_CLC = 0;
			putch(m_counters_send);	// parancskód
			local_CLC++;
			Char_To_Send = m_space;		// Space karakter elv�laszt�nak
			putch(Char_To_Send);
			local_CLC++;

			Num2Dec(Counter.Hosszu);	// hosszu számláló �talak�t�sa ASCII stringg�
			dcmut = 9;
			while(Dec_str[dcmut] == 0) 	// Felesleges bal oldali null�k átlépése
				{
				dcmut--;
				}

			while(dcmut >= 0)	// t�lcsordul�sig megy�nk
				{
				Char_To_Send = Dec_str[dcmut];
				putch(Char_To_Send);
				local_CLC++;
 				dcmut--;
				}
	
			Char_To_Send = m_space;		// Space karakter elv�laszt�nak
			putch(Char_To_Send);
			local_CLC++;
	
			Num2Dec(Counter.Rovid);	// rövid számláló �talak�t�sa ASCII stringg�
			dcmut = 9;
			while(Dec_str[dcmut] == 0) 	// Felesleges bal oldali null�k átlépése
				{
				dcmut--;
				}

			while(dcmut >= 0)	// t�lcsordul�sig megy�nk
				{
				Char_To_Send = Dec_str[dcmut];
				putch(Char_To_Send);
				local_CLC++;
 				dcmut--;
				}
	
			Char_To_Send = m_space;		// Space karakter elv�laszt�nak
			putch(Char_To_Send);
			local_CLC++;

			local_CLC = ~local_CLC + 1;
//			putch(local_CLC);
			Wrts = Wrts + '0';			// ASCII k�dot k�ldjönk
			putch(Wrts);
			Wrts = 0;					// legyen ujra nulla
			putch(ETX);
	

} // end of Send_Counters


/*****************************************************************************
* Setup adatok küldése
* Beolvasás az EEPROM-b�l
****************************************************************************/
void	Send_Setup(void)
{
unsigned char	local_CLC;

	putch(STX);
	local_CLC = 0;
	putch(m_setup_send);
	local_CLC++;
	for(s1=0; s1<setup_size; s1++)
		{
		AddrL = s1 + setup_addr;
		Char_To_Send = CHK_eeprom_read(AddrL);
//		Char_To_Send = Setup_Tomb[s1];      // a setup_tomb byte-jait egyes�vel kik�ldj�k
		putch(Char_To_Send);
		local_CLC++;
		}
	local_CLC = ~local_CLC + 1;
	putch(local_CLC);
	putch(ETX);
}

void	Get_Setup(void)
{

	//	rxoptr++;						// pointer növelés
	//	rxoptr &= SER_FIFO_MASK;		// körbefordulás kezelése

	for (s1=0; s1 < setup_size; s1++)
		{
		AddrL = s1 + setup_addr;		// setup terület cime
//		mpl = s1+2;					// rxfifo offset
		eeprom_irhato = 1;
		Chk = CHK_eeprom_write(AddrL, rxfifo[rxoptr]);	// mpl volt itt
		eeprom_irhato = 0;
		rxoptr++;						// pointer növelése
		rxoptr &= SER_FIFO_MASK;		// körbefordulás kezelése
		}
		rxoptr++;						// CLC 
		rxoptr &= SER_FIFO_MASK;		// körbefordulás kezelése

		putch(STX);					// nyugta
		putch(ACK);
		putch(ETX);


}



/*************************************************
	kikapcsolósi m�veletek
*************************************************/
void		Proc_Shutdown (void)
{
			ermtilt_be();
			erme_lam_off();							// érmeajtó lámpa kikapcs
//			send_mess(str_shutdown);
			mute_be();


			AddrL = setup_credits_addr;					// setup kredit terület cime
			eeprom_irhato = 1;
			Chk = CHK_eeprom_write(AddrL, rxfifo[rxoptr]);	// kiirjuk a kreditek szám�t
			rxoptr++;									// pointer növelése
			rxoptr &= SER_FIFO_MASK;					// körbefordulás kezelése

			AddrL++;									// hanger� állása (+48)
			Chk = CHK_eeprom_write(AddrL, rxfifo[rxoptr]);	// kiirjuk a kreditek szám�t
			eeprom_irhato = 0;
			rxoptr++;									// pointer növelése
			rxoptr &= SER_FIFO_MASK;					// körbefordulás kezelése

			rxoptr++;									// ETX átlépése
			rxoptr &= SER_FIFO_MASK;					// körbefordulás kezelése

			Dec_RX_Msg();							// üzenet kezelve

			Delayms(200);		////*/*/**/*/*/*/**100-500
			amptap_ki();
			Delayms(400);		// egy kis id� legyen a tranziensek k�z�tt 200-500
			montap_ki();
			Delayms(400);		// egy kis id� legyen a tranziensek k�z�tt 200-500
			run_led_off();
	
//			if(SFbits.Long_Switchoff)
//				{ s1 = 30; } else { s1 = 10; }  // original
		//		{ s1 = 50; } else { s1 = 20; }  // hosszabb kikapcsolós kell	{ s1 =30- 50; } else { s1 =10- 20; }mmmmmmm

				s1 = 40;
			
			while(s1 > 0)
				{
				sleep_led_off();
				erme_lam_off();
				kibe_lam_off();
				Delayms(500);
				s2 = PC12V();			// ha m�r kikapcsolat a t�p, hamarabb kil�p�nk	

				sleep_led_on();
				erme_lam_on();
				kibe_lam_on();
				Delayms(500);

				s1--;
				if (!s2)				// régi hw-n�l az s2 sose lesz 0
					{
					s1 = 0;
					SFbits.Normal_Switchoff = 1;		// jelezzük, hogy mag�t�l �llt le a PC
					}			// v�ge legyen a while-nak (break)
				} //while
			

		if (!SFbits.Normal_Switchoff)		// ha nem �llt le mag�t�l, hossz� ATX impulzus
			{	

			if (!oldhw)
				{  // ujhw	esetén hossz� atx impulzust adunk
				atxst_be();				// atx t�ske
				for(s1=0; s1<11; s1++)
					{
					err_led_on();				// hibaled bekapcs
					Delayms(250);	
					err_led_off();				// hibaled bekapcs
					Delayms(250);		
					} // for ciklus
				} // end of !oldhw
			} // if sfbits.normal_switchoff
		
			sleep_led_off();
			erme_lam_off();
			kibe_lam_off();
			Delayms(500);		//	mmmmmmmmmm
			pctap_ki();
			Delayms(500);		//	mmmmmmmmmm
			atxst_ki();				// atzx t�ske vissza, m�r valszeg le�llt
			beep(off);				// hangjelzés


	//		RESET();   ////// mod 2010 07 09
	//		#asm												
	//		asm("BCF _PCLATH,4");									
	//		asm("BCF _PCLATH,3");									
	//		asm("GOTO 0x0000");		// restart, a HW újraindul		
	//		#endasm
}
	

/***********************************************************************************
* érme bedob�st vizsg�l� rutin
V�gig k�rdezi a 6 csatorn�t, ha sehol semmi, hamar visszat�r
Ha valahol van érme bedob�s, akkor az �rz�kelt jel után 5ms prellmentesit�si idot várunk,
utána újra olvassuk a l�bat. Ha m�g mindig megvan, üzenetet k�ld�nk, és megvárjuk a felfut� �lt,
azt is prellmentesitve (5 ms).
************************************************************************************/
void	Scan_Inputs(void)
{
unsigned char	sci;
// érmevizsgáló csatorn�k figyel�se

//		mute_be();				//csak teszt

// ha minden gomb meg van nyomva egyszerre, akkor t�pfesz elmen�s van folyamatban

		
			sci=0;
			sci = sci+  PageRight_Pressed();
			sci = sci+ StepLeft_Pressed();
			sci = sci+ StepRight_Pressed();
			sci =  sci+ StepRight_Pressed();
			sci = sci+  Enter_Pressed();
			sci = sci+ Return_Pressed();
		
			if(SFbits.Remote_Present)		// csak akkor n�z�nk távvezérlőt, ha van
				{
				sci = sci+ Remote_Volup();
				sci = sci+ Remote_Voldn();
				}

			if (sci > 3 )
				{
				eeprom_irhato = 0;				// engedélyezzük aqz eeprom írást
				Delayms(500);
				}
		

		s1 = CH1_Coin();
		if (s1 != ERMbits.CH1)      // ha igaz a feltétel, �lv�lt�s volt 		
			{
			Delayms(PrellDelay);				// 5 ms prellmentesit�s
			s1 = CH1_Coin();			// újra r�olvasunk
			if (s1 != ERMbits.CH1)      // ha m�g mindig igaz a feltétel, biztos az �lv�lt�s 	
				{
				if(s1 ==1)					// lefut� �l j�tt, érme �rkezik
					{
					ERMbits.CH1 = 1;		// biztosan megj�tt az érme jel
//					bor();		////BOR figyeles
					send_mess(m_ch1_Pressed);	// üzenetet k�ld�nk
					if(!TVbits.PC_Test_Mode)	// Teszt módban nem számoljuk az �rm�ket
						{
//						bor();		////BOR figyeles
						Counter.Hosszu += CH_Money[1];
						Counter.Rovid += CH_Money[1];
						EEPROM_Counters_Write();	// kiirjuk az uj értéket
						}
					} else
					{
					ERMbits.CH1 = 0;		// v�get �rt az érme jel
					send_mess(m_ch1_Released);	// üzenetet k�ld�nk
					}
				}
			} 

		s1 = CH2_Coin();
		if (s1 != ERMbits.CH2)      // ha igaz a feltétel, �lv�lt�s volt 		
			{
			Delayms(PrellDelay);				// 5 ms prellmentesit�s
			s1 = CH2_Coin();			// újra r�olvasunk
			if (s1 != ERMbits.CH2)      // ha m�g mindig igaz a feltétel, biztos az �lv�lt�s 	
				{
				if(s1 ==1)					// lefut� �l j�tt, érme �rkezik
					{
					ERMbits.CH2 = 1;		// biztosan megj�tt az érme jel
//					bor();		////BOR figyeles
					send_mess(m_ch2_Pressed);	// üzenetet k�ld�nk
					if(!TVbits.PC_Test_Mode)	// Teszt módban nem számoljuk az �rm�ket
						{
							bor();		////BOR figyeles
						Counter.Hosszu += CH_Money[2];
						Counter.Rovid += CH_Money[2];
						EEPROM_Counters_Write();	// kiirjuk az uj értéket
						}
					} else
					{
					ERMbits.CH2 = 0;		// v�get �rt az érme jel
					send_mess(m_ch2_Released);	// üzenetet k�ld�nk
					}
				}
			} 


		s1 = CH3_Coin();
		if (s1 != ERMbits.CH3)      // ha igaz a feltétel, �lv�lt�s volt 		
			{
			Delayms(PrellDelay);				// 5 ms prellmentesit�s
			s1 = CH3_Coin();			// újra r�olvasunk
			if (s1 != ERMbits.CH3)      // ha m�g mindig igaz a feltétel, biztos az �lv�lt�s 	
				{
				if(s1 ==1)					// lefut� �l j�tt, érme �rkezik
					{
					ERMbits.CH3 = 1;		// biztosan megj�tt az érme jel
//					bor();		////BOR figyeles
					send_mess(m_ch3_Pressed);	// üzenetet k�ld�nk
					if(!TVbits.PC_Test_Mode)	// Teszt módban nem számoljuk az �rm�ket
						{
//						bor();		////BOR figyeles
						Counter.Hosszu += CH_Money[3];
						Counter.Rovid += CH_Money[3];
						EEPROM_Counters_Write();	// kiirjuk az uj értéket
						}
					} else
					{
					ERMbits.CH3 = 0;		// v�get �rt az érme jel
					send_mess(m_ch3_Released);	// üzenetet k�ld�nk
					}
				}
			} 


		s1 = CH4_Coin();
		if (s1 != ERMbits.CH4)      // ha igaz a feltétel, �lv�lt�s volt 		
			{
			Delayms(PrellDelay);				// 5 ms prellmentesit�s
			s1 = CH4_Coin();			// újra r�olvasunk
			if (s1 != ERMbits.CH4)      // ha m�g mindig igaz a feltétel, biztos az �lv�lt�s 	
				{
				if(s1 ==1)					// lefut� �l j�tt, érme �rkezik
					{
					ERMbits.CH4 = 1;		// biztosan megj�tt az érme jel
//					bor();		////BOR figyeles
					send_mess(m_ch4_Pressed);	// üzenetet k�ld�nk
					if(!TVbits.PC_Test_Mode)	// Teszt módban nem számoljuk az �rm�ket
						{
//						bor();		////BOR figyeles
						Counter.Hosszu += CH_Money[4];
						Counter.Rovid += CH_Money[4];
						EEPROM_Counters_Write();	// kiirjuk az uj értéket
						}
					} else
					{
					ERMbits.CH4 = 0;		// v�get �rt az érme jel
					send_mess(m_ch4_Released);	// üzenetet k�ld�nk
					}
				}
			} 


		s1 = CH5_Coin();
		if (s1 != ERMbits.CH5)      // ha igaz a feltétel, �lv�lt�s volt 		
			{
			Delayms(PrellDelay);				// 5 ms prellmentesit�s
			s1 = CH5_Coin();			// újra r�olvasunk
			if (s1 != ERMbits.CH5)      // ha m�g mindig igaz a feltétel, biztos az �lv�lt�s 	
				{
				if(s1 ==1)					// lefut� �l j�tt, érme �rkezik
					{
					ERMbits.CH5 = 1;		// biztosan megj�tt az érme jel
//					bor();		////BOR figyeles
					send_mess(m_ch5_Pressed);	// üzenetet k�ld�nk
					if(!TVbits.PC_Test_Mode)	// Teszt módban nem számoljuk az �rm�ket
						{
//						bor();		////BOR figyeles
						Counter.Hosszu += CH_Money[5];
						Counter.Rovid += CH_Money[5];
						EEPROM_Counters_Write();	// kiirjuk az uj értéket
						}
					} else
					{
					ERMbits.CH5 = 0;		// v�get �rt az érme jel
					send_mess(m_ch5_Released);	// üzenetet k�ld�nk
					}
				}
			} 


		s1 = CH6_Coin();
		if (s1 != ERMbits.CH6)      // ha igaz a feltétel, �lv�lt�s volt 		
			{
			Delayms(PrellDelay);				// 5 ms prellmentesit�s
			s1 = CH6_Coin();			// újra r�olvasunk
			if (s1 != ERMbits.CH6)      // ha m�g mindig igaz a feltétel, biztos az �lv�lt�s 	
				{
				if(s1 ==1)					// lefut� �l j�tt, érme �rkezik
					{
					ERMbits.CH6 = 1;		// biztosan megj�tt az érme jel
//					bor();		////BOR figyeles
					send_mess(m_ch6_Pressed);	// üzenetet k�ld�nk
					if(!TVbits.PC_Test_Mode)	// Teszt módban nem számoljuk az �rm�ket
						{
//						bor();		////BOR figyeles
						Counter.Hosszu += CH_Money[6];
						Counter.Rovid += CH_Money[6];
						EEPROM_Counters_Write();	// kiirjuk az uj értéket
						}
					} else
					{
					ERMbits.CH6 = 0;		// v�get �rt az érme jel
					send_mess(m_ch6_Released);	// üzenetet k�ld�nk
					}
				}
			} 


sci=0;
			sci = sci+  PageRight_Pressed();
			sci = sci+ StepLeft_Pressed();
			sci = sci+ StepRight_Pressed();
			sci =  sci+ StepRight_Pressed();
			sci = sci+  Enter_Pressed();
			sci = sci+ Return_Pressed();
		
			if(SFbits.Remote_Present)		// csak akkor n�z�nk távvezérlőt, ha van
				{
				sci = sci+ Remote_Volup();
				sci = sci+ Remote_Voldn();
				}

			if (sci > 3 )
				{
				eeprom_irhato = 0;				// engedélyezzük aqz eeprom írást
				Delayms(500);
				}

//   bor();		////BOR figyeles
// érmeajtó figyel�se
	s1 = ERMEAJTO_Nyitva();
		if (s1 != ERMbits.Erme_Ajto)      // ha igaz a feltétel, �lv�lt�s volt 		
			{
			Delayms(PrellDelay);						// 5 ms prellmentesit�s
			s1 = ERMEAJTO_Nyitva();			// újra r�olvasunk
			if (s1 != ERMbits.Erme_Ajto)      		// ha m�g mindig igaz a feltétel, biztos az �lv�lt�s 	
				{
				if(s1 ==1)						// lefut� �l j�tt, érmeajtót kinyitott�k
					{
					ERMbits.Erme_Ajto = 1;			// r�gz�tj�k, hogy nyitva az érmeajtó
					send_mess(m_Vol_Pressed);		// üzenetet k�ld�nk
					} else
					{
					ERMbits.Erme_Ajto = 0;			// bezérték az érmeajtót
					send_mess(m_Vol_Released);	// üzenetet k�ld�nk
					}
				}
			} 

// bor();		////BOR figyeles
// kulcsos kapcsolo figyel�se
		s1 = SETUP_Pressed();
		if (s1 != ERMbits.Serv_Mode)      // ha igaz a feltétel, �lv�lt�s volt 		
			{
			Delayms(PrellDelay);				// 5 ms prellmentesit�s
			s1 = SETUP_Pressed();			// újra r�olvasunk
			if (s1 != ERMbits.Serv_Mode)      // ha m�g mindig igaz a feltétel, biztos az �lv�lt�s 	
				{
				if(s1 ==1)								// lefut� �l j�tt, elforditott�k a kulcsos kapcsolót
					{
					ERMbits.Serv_Mode = 1;			// r�gzitj�k, hogy a kulcsos kapcsoló el van forditva
					send_mess(m_Service_Pressed);		// üzenetet k�ld�nk
					beep(serv);						// service mód pittyeg�s
					} else
					{
					ERMbits.Serv_Mode = 0;			// a kulcsos kapcsoló alaphelyzetbe �llt
					send_mess(m_Service_Released);	// üzenetet k�ld�nk
					}
				}
			} 
// bor();		////BOR figyeles
	
//  nyomógombok figyel�se
	
		s1 = PageLeft_Pressed();
		if (s1)					// ha nyomva van, n�velj�k a számlálót 
			{sci++;
			RM_Count++;
			if ((RM_Count > RMC_First) && (BUTbits.Page_Left))
				{
				send_mess(m_PLeft_Pressed);
				RM_Count = Button_Repeat;
				}
			}
		if (s1 != BUTbits.Page_Left)      // ha igaz a feltétel, �lv�lt�s volt 		
			{
			Delayms(PrellDelay);				// 5 ms prellmentesit�s
			s1 = PageLeft_Pressed();			// újra r�olvasunk
			if (s1 != BUTbits.Page_Left)      // ha m�g mindig igaz a feltétel, biztos az �lv�lt�s 	
				{
				if(s1 ==1)					// lefut� �l j�tt, érme �rkezik
					{
					BUTbits.Page_Left = 1;		// biztosan megj�tt az érme jel
					PageLeftLam_be();
					send_mess(m_PLeft_Pressed);	// üzenetet k�ld�nk
					RM_Count = 0;
					} else
					{
					BUTbits.Page_Left = 0;		// v�get �rt az érme jel
					PageLeftLam_ki();
					RM_Count = 0;		
					send_mess(m_PLeft_Released);	// üzenetet k�ld�nk
					}
				}
			} 
// bor();		////BOR figyeles

sci=0;
			sci = sci+  PageRight_Pressed();
			sci = sci+ StepLeft_Pressed();
			sci = sci+ StepRight_Pressed();
			sci =  sci+ StepRight_Pressed();
			sci = sci+  Enter_Pressed();
			sci = sci+ Return_Pressed();
		
			if(SFbits.Remote_Present)		// csak akkor n�z�nk távvezérlőt, ha van
				{
				sci = sci+ Remote_Volup();
				sci = sci+ Remote_Voldn();
				}

			if (sci > 3 )
				{
				eeprom_irhato = 0;				// engedélyezzük aqz eeprom írást
				Delayms(500);
				}

		s1 = StepLeft_Pressed();
		if (s1)					// ha nyomva van, n�velj�k a számlálót 
			{
			RM_Count++;
			if ((RM_Count > RMC_First) && (BUTbits.Step_Left))
				{
				send_mess(m_SLeft_Pressed);
				RM_Count = Button_Repeat;
				if(TVbits.PC_Test_Mode)				// bizonyos esetekben (teszt, setup) pittyeg a gomb
					{
					beep(ok);
					}	
				}
			}

//  bor();		////BOR figyeles
		if (s1 != BUTbits.Step_Left)      // ha igaz a feltétel, �lv�lt�s volt 		
			{
			Delayms(PrellDelay);				// 5 ms prellmentesit�s
			s1 = StepLeft_Pressed();			// újra r�olvasunk
			if (s1 != BUTbits.Step_Left) 				     // ha m�g mindig igaz a feltétel, biztos az �lv�lt�s 	
				{
				if(s1 ==1)								// lefut� �l j�tt, érme �rkezik
					{
					BUTbits.Step_Left = 1;				// állapot bejegyz�se
					StepLeftLam_be();
					send_mess(m_SLeft_Pressed);		// üzenetet k�ld�nk
					RM_Count = 0;
					if(TVbits.PC_Test_Mode)				// bizonyos esetekben (teszt, setup) pittyeg a gomb
						{
						beep(ok);
						}	

					} else
					{
					BUTbits.Step_Left = 0;				// v�get �rt az érme jel
					StepLeftLam_ki();
					send_mess(m_SLeft_Released);		// üzenetet k�ld�nk
					RM_Count = 0;
					}
				}
			} 

// bor();		////BOR figyeles
		s1 = Return_Pressed();
		if (s1 != BUTbits.Return)      // ha igaz a feltétel, �lv�lt�s volt 		
			{
			Delayms(PrellDelay);				// 5 ms prellmentesit�s
			s1 = Return_Pressed();			// újra r�olvasunk
			if (s1 != BUTbits.Return)      // ha m�g mindig igaz a feltétel, biztos az �lv�lt�s 	
				{
				if(s1 ==1)					// lefut� �l j�tt, érme �rkezik
					{
					BUTbits.Return = 1;		// biztosan megj�tt az érme jel
					ReturnLam_be();
					send_mess(m_GBack_Pressed);	// üzenetet k�ld�nk

					} else
					{
					BUTbits.Return = 0;						// v�get �rt az érme jel
					ReturnLam_ki();
					send_mess(m_GBack_Released);			// üzenetet k�ld�nk
					}
				}
			} 
// bor();		////BOR figyeles	
		s1 = Enter_Pressed();
		if (s1 != BUTbits.Enter)      // ha igaz a feltétel, �lv�lt�s volt 		
			{
			Delayms(PrellDelay);				// 5 ms prellmentesit�s
			s1 = Enter_Pressed();			// újra r�olvasunk
			if (s1 != BUTbits.Enter)      // ha m�g mindig igaz a feltétel, biztos az �lv�lt�s 	
				{
				if(s1 ==1)					// lefut� �l j�tt, érme �rkezik
					{
					BUTbits.Enter = 1;		// biztosan megj�tt az érme jel
					EnterLam_be();
					send_mess(m_GEnter_Pressed);	// üzenetet k�ld�nk

					} else
					{
					BUTbits.Enter = 0;		// v�get �rt az érme jel
					EnterLam_ki();
					send_mess(m_GEnter_Released);	// üzenetet k�ld�nk
					}
				}
			} 
// bor();		////BOR figyeles
		s1 = StepRight_Pressed();
		if (s1)					// ha nyomva van, n�velj�k a számlálót 
			{
			RM_Count++;
			if ((RM_Count > RMC_First) && (BUTbits.Step_Right))
				{
				send_mess(m_SRight_Pressed);
				RM_Count = Button_Repeat;
				if(TVbits.PC_Test_Mode)				// bizonyos esetekben (teszt, setup) pittyeg a gomb
					{
					beep(ok);
					}	
		}
			}

// bor();		////BOR figyeles
		if (s1 != BUTbits.Step_Right)      // ha igaz a feltétel, �lv�lt�s volt 		
			{
			Delayms(PrellDelay);				// 5 ms prellmentesit�s
			s1 = StepRight_Pressed();			// újra r�olvasunk
			if (s1 != BUTbits.Step_Right)      // ha m�g mindig igaz a feltétel, biztos az �lv�lt�s 	
				{
				if(s1 ==1)					// lefut� �l j�tt, érme �rkezik
					{
					BUTbits.Step_Right = 1;		// biztosan megj�tt az érme jel
					StepRightLam_be();
					send_mess(m_SRight_Pressed);	// üzenetet k�ld�nk
					RM_Count = 0;
					if(TVbits.PC_Test_Mode)				// bizonyos esetekben (teszt, setup) pittyeg a gomb
						{
						beep(ok);
						}	
					} else
					{
					BUTbits.Step_Right = 0;				// v�get �rt az érme jel
					StepRightLam_ki();
					send_mess(m_SRight_Released);		// üzenetet k�ld�nk
					RM_Count = 0;
					}
				}
			} 

// bor();		////BOR figyeles
		s1 = PageRight_Pressed();
		if (s1)					// ha nyomva van, n�velj�k a számlálót 
			{
			RM_Count++;
			if ((RM_Count > RMC_First) && (BUTbits.Page_Right))
				{
				send_mess(m_PRight_Pressed);
				RM_Count = Button_Repeat;
				}
			}

// bor();		////BOR figyeles
		if (s1 != BUTbits.Page_Right)      // ha igaz a feltétel, �lv�lt�s volt 		
			{
			Delayms(PrellDelay);				// 5 ms prellmentesit�s
			s1 = PageRight_Pressed();			// újra r�olvasunk
			if (s1 != BUTbits.Page_Right)      // ha m�g mindig igaz a feltétel, biztos az �lv�lt�s 	
				{
				if(s1 ==1)					// lefut� �l j�tt, érme �rkezik
					{
					BUTbits.Page_Right = 1;		// biztosan megj�tt az érme jel
					PageRightLam_be();
					send_mess(m_PRight_Pressed);	// üzenetet k�ld�nk
					RM_Count = 0;
					} else
					{
					BUTbits.Page_Right = 0;		// v�get �rt az érme jel
					PageRightLam_ki();
					RM_Count = 0;
					send_mess(m_PRight_Released);	// üzenetet k�ld�nk
					}
				}
			} 


// Távvezérlő jelek figyel�se
// TV0 - 4ch: MUTE, 2ch VOLUP
// TV1 - 4ch:  VOLUP, 2ch: VOLDOWN
// TV2 - 4ch: VOLDN, 2ch : NC
// TV3 - 4ch: 4.GOMB, 2ch: NC

// bor();		////BOR figyeles
	if(SFbits.Remote_Present)		// csak akkor n�z�nk távvezérlőt, ha van
		{

		s1 = Remote_Volup();
		if (s1)					// ha nyomva van, n�velj�k a számlálót 
			{
			RM_Count++;
//			if ((RM_Count > 25) && (TVbits.Volup) && ((RM_Count & 512) == 1))
			if ((RM_Count > RMC_First) && (TVbits.Volup))
				{
				send_mess(m_Volup_Pressed);
				RM_Count = RMC_Repeat;
				}
			}

// bor();		////BOR figyeles
		if (s1 != TVbits.Volup)      // ha igaz a feltétel, �lv�lt�s volt 		
			{
			Delayms(PrellDelay);				// 5 ms prellmentesit�s
			s1 = Remote_Volup();			// újra r�olvasunk
			if (s1 != TVbits.Volup)      // ha m�g mindig igaz a feltétel, biztos az �lv�lt�s 	
				{
				if(s1 ==1)					// lefut� �l j�tt, érme �rkezik
					{
					TVbits.Volup = 1;		// biztosan megj�tt az érme jel
					send_mess(m_Volup_Pressed);	// üzenetet k�ld�nk
					RM_Count = 0;
					} else
					{
					TVbits.Volup = 0;		// v�get �rt az érme jel
					RM_Count = 0;
					send_mess(m_Volup_Released);	// üzenetet k�ld�nk
					}
				}
			} 

// bor();		////BOR figyeles
		s1 = Remote_Voldn();
		if (s1)					// ha nyomva van, n�velj�k a számlálót 
			{
			RM_Count++;
//			if ((RM_Count > 25) && (TVbits.Volup) && ((RM_Count & 512) == 1))
			if ((RM_Count > RMC_First) && (TVbits.Voldn))
				{
				send_mess(m_Voldn_Pressed);
				RM_Count = RMC_Repeat;
				}
			}
		if (s1 != TVbits.Voldn)      // ha igaz a feltétel, �lv�lt�s volt 		
			{
			Delayms(PrellDelay);				// 5 ms prellmentesit�s
			s1 = Remote_Voldn();			// újra r�olvasunk
			if (s1 != TVbits.Voldn)      // ha m�g mindig igaz a feltétel, biztos az �lv�lt�s 	
				{
				if(s1 ==1)					// lefut� �l j�tt, érme �rkezik
					{
					TVbits.Voldn = 1;		// biztosan megj�tt az érme jel
					send_mess(m_Voldn_Pressed);	// üzenetet k�ld�nk
					RM_Count = 0;
					} else
					{
					TVbits.Voldn = 0;		// v�get �rt az érme jel
					RM_Count = 0;
					send_mess(m_Voldn_Released);	// üzenetet k�ld�nk
					}
				}
			} 

// Mute és 4. gomb csak a 4ch távvezérlők�n van

// bor();		////BOR figyeles
	if(!SFbits.Remote_2ch)
		{
		s1 = Remote_Mute();
		if (s1 != TVbits.Mute)      // ha igaz a feltétel, �lv�lt�s volt 		
			{
			Delayms(PrellDelay);				// 5 ms prellmentesit�s
			s1 = Remote_Mute();			// újra r�olvasunk
			if (s1 != TVbits.Mute)      // ha m�g mindig igaz a feltétel, biztos az �lv�lt�s 	
				{
				if(s1 ==1)					// lefut� �l j�tt, érme �rkezik
					{
					TVbits.Mute = 1;		// biztosan megj�tt az érme jel
					send_mess(m_Volmut_Pressed);	// üzenetet k�ld�nk
					send_mess(ACK);	// üzenetet k�ld�nk

					} else
					{
					TVbits.Mute = 0;		// v�get �rt az érme jel
					send_mess(m_Volmut_Released);	// üzenetet k�ld�nk
					}
				}
			} 
// bor();		////BOR figyeles
		s1 = Remote_Vol4b();
		if (s1 != TVbits.Vol4b)      // ha igaz a feltétel, �lv�lt�s volt 		
			{
			Delayms(PrellDelay);				// 5 ms prellmentesit�s
			s1 = Remote_Vol4b();			// újra r�olvasunk
			if (s1 != TVbits.Vol4b)      // ha m�g mindig igaz a feltétel, biztos az �lv�lt�s 	
				{
				if(s1 ==1)					// lefut� �l j�tt, érme �rkezik
					{
					TVbits.Vol4b = 1;		// biztosan megj�tt az érme jel
					send_mess(m_vol4b_Pressed);	// üzenetet k�ld�nk

					} else
					{
					TVbits.Vol4b = 0;		// v�get �rt az érme jel
					send_mess(m_vol4b_Released);	// üzenetet k�ld�nk
					}
				}
			} 
		} // Remote_2ch
	} // Remote_Present

// kikapcsolóst kezdem�nyeztek?
	s1 = KIBE_Pressed();
		if (s1 != BUTbits.Ki_Be)      // ha igaz a feltétel, �lv�lt�s volt 		
			{
			Delayms(PrellDelay);				// 5 ms prellmentesit�s
			s1 = KIBE_Pressed();			// újra r�olvasunk
			if (s1 != BUTbits.Ki_Be)      // ha m�g mindig igaz a feltétel, biztos az �lv�lt�s 	
				{
				if(s1 ==1)					// lefut� �l j�tt, érme �rkezik
					{
					BUTbits.Ki_Be = 1;		// biztosan megj�tt az érme jel
					send_mess(m_shutdown_rq);	// üzenetet k�ld�nk
					beep(off);
					Delayms(50);				// v�rakozunk, hogy j�jjön a kikapcs üzenet
		//			Proc_Shutdown();
					} else
					{
					BUTbits.Ki_Be = 0;		// v�get �rt az érme jel
					}
				}
			} 

//	mute_ki();	 // csak teszt
}


/*********************************************************************************
* számlálók null�z�sa
* jelszóval v�dett
* STX-'n'-Ev1-Ev2-Ho1-Ho2-Nap1-Nap2-P1-P2-P3-P4-CLC-ETX
*********************************************************************************/
void	Counter_Reset(void)
{

		CR_Mpl =  (rxfifo[rxoptr]-'0') * 1000;
		rxoptr++;
		rxoptr &= SER_FIFO_MASK;
	 	CR_Mpl +=  (rxfifo[rxoptr]-'0') * 100;
		rxoptr++;
		rxoptr &= SER_FIFO_MASK;
		CR_Mpl +=  (rxfifo[rxoptr]-'0') * 10; 
		rxoptr++;
		rxoptr &= SER_FIFO_MASK;
		CR_Mpl +=  (rxfifo[rxoptr]-'0');  // dátum, Ev-ho, pl. 1005
		rxoptr += 3;					// növelés, plusz k�t karaktert (nap) �tl�p�nk
		rxoptr &= SER_FIFO_MASK;
		CR_Mpl += 123; 

		 DateStr[0] = CHK_eeprom_read(12);    // byte-onként beolvassuk a szevizd második fel�t
		 DateStr[1] = CHK_eeprom_read(13);    // byte-onként beolvassuk a szevizd második fel�t
		 DateStr[2] = CHK_eeprom_read(14);    // byte-onként beolvassuk a szevizd második fel�t
		 DateStr[3] = CHK_eeprom_read(15);    // byte-onként beolvassuk a szevizd második fel�t


		CR_Pass =  (DateStr[0]-'0') * 1000 + (DateStr[1]-'0') * 100 + (DateStr[2]-'0') * 10 + (DateStr[3]-'0');  // Szervizkod
		CR_Mpl *= CR_Pass;
		CR_Mpl = CR_Mpl % 10000;			// az utolsó n�gy számjegy kell


		CR_Pass = (rxfifo[rxoptr]-'0') * 1000;
		rxoptr++;
		rxoptr &= SER_FIFO_MASK;
		CR_Pass += (rxfifo[rxoptr]-'0') * 100;
		rxoptr++;
		rxoptr &= SER_FIFO_MASK;
		CR_Pass += (rxfifo[rxoptr]-'0') * 10;
		rxoptr++;
		rxoptr &= SER_FIFO_MASK;
	 	CR_Pass += (rxfifo[rxoptr]-'0');
		rxoptr++;
		rxoptr &= SER_FIFO_MASK;
		
		if(CR_Pass == CR_Mpl)			// megegyezik a k�ld�tt és a szamitott jelszo
			{
									// ha nincs a három t�rolt jelszo k�z�tt, null�zhatunk
									// ha null�zunk, a használt jelszot ki kell irni, a pointert mozgatni.
			AddrL = pass_area;		// a t�rolt jelszavak beolvasás�hoz a Booking_Counter-t használjuk
			for(s1= 0;  s1 < 6; s1++)	
				{
				Booking_Counter.cnumbyte[s1] = CHK_eeprom_read(AddrL);
				AddrL++;
				}

			s2 = 1;						// =1, ha m�g nem használtuk a jelszót
		 	s3 = CHK_eeprom_read(passptr_addr);    // a következő cim, ahov� a használt jelszó kiirhato, s3-ban a passptr	
			if (Booking_Counter.Hosszu_LInt == CR_Mpl) { s2 = 0; }			// m�r használtuk ezt a jelszot
			if (Booking_Counter.Hosszu_MInt == CR_Mpl) { s2 = 0; }	// m�r használtuk ezt a jelszot
			if (Booking_Counter.Rovid_LInt == CR_Mpl) { s2 = 0; }	// m�r használtuk ezt a jelszot
				
				if (s2)		// s2 = 1, ha jo jelszot k�ldtek, és m�g nem használtuk
					{

				switch(s3)		// csak akkor irjuk ki a kapott jelszot, ha jo, és m�g nem használtuk
				{
				case 250: 	{ 
					eeprom_irhato = 1;
					Booking_Counter.Hosszu_LInt = CR_Mpl;
					CHK_eeprom_write(pass_area, Booking_Counter.cnumbyte[0]);			
					CHK_eeprom_write(pass_area + 1, Booking_Counter.cnumbyte[1]);			
					s3 = 252;
					CHK_eeprom_write(passptr_addr, s3);			
					eeprom_irhato = 0;
					}
				break;
		
				case 252:		{  
					Booking_Counter.Hosszu_MInt = CR_Mpl;
					eeprom_irhato = 1;
					CHK_eeprom_write(pass_area +2, Booking_Counter.cnumbyte[2]);			
					CHK_eeprom_write(pass_area +3, Booking_Counter.cnumbyte[3]);			
					s3 = 254;
					CHK_eeprom_write(passptr_addr, s3);			
					eeprom_irhato = 0;
}
				break;

				case 254: 	{ 
					Booking_Counter.Rovid_LInt = CR_Mpl;
					eeprom_irhato = 1;
					CHK_eeprom_write(pass_area + 4, Booking_Counter.cnumbyte[4]);			
					CHK_eeprom_write(pass_area + 5, Booking_Counter.cnumbyte[5]);			
					s3 = 250;
					CHK_eeprom_write(passptr_addr, s3);			
					eeprom_irhato = 0;
}
				break;
				} // switch

					Counter.Hosszu = 0;				// nullázzuk a számlálókat
					Counter.Rovid = 0;
					EEPROM_Counters_Write();			// kiirjuk az uj értéket
					putch(STX);				// null�ztunk, pozitiv nyugta
					putch(ACK);
					putch(ETX);		
					}	else
					{
					putch(STX);				// nem null�ztunk, negativ nyugta
					putch(NAK);
					putch(ETX);
					}	// nem null�ztunk használt jelszo miatt
			}
			else
			{

			putch(STX);				// jelszavak nem egyeznek meg, nem null�ztunk, negativ nyugta
			putch(NAK);
			putch(ETX);

			} // end of if	

			rxoptr++;						// ETX �tl�ptet�se
			rxoptr &= SER_FIFO_MASK;
			Dec_RX_Msg();					// üzenet kezelve


} // end of Counter_Reset

/**********************************************************************************
* Távvezérlő jelenl�t�nek vizsg�lata
**********************************************************************************/
void		Remote_Present(void)	
{
		SFbits.Remote_Present = 1;			// most ugy vessz�k, hogy van, nullázzuk, ha nincs
		s1 = Remote_Volup();
	 		if(s1)
				{
				s1 = Remote_Voldn();
				if(s1)
					{
					SFbits.Remote_Present = 0;  // mind a k�t bemenet magas, nincs távvezérlő
					} // Voldn
				} // Volup
	
}  // Remote_Present




/**********************************************************************************
*  F�program
*
*********************************************************************************/
			// ide ugrik reset után
void main (void)

{
	di();					// megszakitas tiltas az elejen
	Delayms(100);	
	TRISC = 0b10111111;	// megnezzuk, milyen hardwareben van a chip
	Delayms(50);
	oldhw = 1;		
	oldhw = RC5;			// lenull�zza, ha uj hardware
//	Delayms(50);
//	txfifo[56] = 'A';

if	(oldhw)
	{
	Init_Old_Ports();
	} else
	{
	Init_New_Ports();
	}

	Init_HW();					// Innent�l egyforma a régi és az uj hw kezelése

	init_usart();


	SPEN = 0;			// egyelére tiltjuk a felkonfigur�lt soros portot
	TXEN = 0;			// tiltjuk a soros ad�st
	TXIE = 0;				// tiltjuk az ad�s megszakit�s kérés�t

	// Timer 0 init
	T0CS=0;	//Internal instruction cycle clock
	PSA=0;	//Prescaler is assigned to the Timer0 module
	PS0=1;	//Prescaler 256
	PS1=1;
	PS2=1;
	T0IE= 0;			//TIMER0 megszakitas engedélyezés 
	TMR0IE = 0;
	T0IF= 0;			//esetlegesTIMER0 megszakitas törlés	
//timer1 init

// T1CON = 0b00000001;	//TIMER1_ON_16BIT_ T0CKI _ ELO OSZTO 1_FOSC/4
	TMR1IE=0;			//TIMER1 megszakitas engedélyezés 
BOR=1;
POR=1;
RCIF=0;
//	SFbits.In_Msg = 0;				// =1 ha �pp van bejövő üzenet
//	SFbits.Clear_Credit = 0;			// Alaphelyzetben nincs kerdit törlés, tesztre van, a gombnyomás hossza állítja
//	SFbits.Keyboard_Enable = 0;		// Alaphelyzetben nincs bill. t�mogat�s DIP kapcsoló állítja
//	SFbits.Remote_Present = 0;		// =1, ha van bedugva távvezérlő a foglalatba
	SFbits.x	= 0;			// inicializ�l�s

	RX_Msg = 0;				//  M�g nincs bejövő üzenet
	ERMbits.x = 0;					// bemeneti állapotok nyilvántart�s�nak törlése
	BadButBits.x = 64;				// nyomógombok állapot�nak nyilvántart�sa
	BUTbits.x = 0;					// nyomógombok állapot�nak nyilvántart�sa
	TVbits.x = 0;					// Távvezérlő gombok állapot�nak nyilvántart�sa
	TVbits.PC_Logged = 0;			// m�g nem jelentkezett be a pc; (a BUTbits.x ezt is null�zza)
	Wrts = 0;						// Error uzenet, kezdőértéke nulla
//	Counter.Hosszu = 0x11223344;
//	Counter.Rovid = 0x55667788;

	s1 = RCREG;
	s1 = RCREG;
	s1 = RCREG;			// a kezdeti bejövő k�bor karaktereket eldobjuk
	rxiptr = rxoptr = 0;
	RCIF = 0;				// ha volt is karakter, t�r�lj�k

	SPEN = 1;			// engedélyezzük soros portot
	TXEN = 1;			// engedélyezzük a soros ad�st

	ei();					// engedélyezzük a megszakit�sokat
 
// KI-BE gomb kikerül�se

	s1 =  0;						// ciklusváltoz� init
	while (s1 < 50)					// 50-szer r�olvasunk
		{
		s2 = KIBE_Pressed();		// várjuk, hogy megnyomj�k a ki-be gombot
		s1++;
		if (s2 == 0)				// ha nincs megnyomva, nullázzuk a ciklusváltoz�t
			{
			s1 = 0;
			} else
			{
			Delayms(2);			//  50 * 2 ms = 100 ms, legal�bb eddig nyomni kell a be gombot
			} // end of if
		} // end of while
	
		erme_lam_on();			// érmeajtó lámpa bekapcs
		kibe_lam_on();				// h�ts� nyomógomb lámpa bekapcs
		beep(ok);					// KI-BE gomb megnyomás�nak jelzése

// Nyomj�k a KI-BE gombot, megnézzük, meddig nyomj�k
	s1 =  0;						// ciklusváltoz� init
	s2 = KIBE_Pressed();			// várjuk, hogy megnyomj�k a ki-be gombot
	while (s2)						// amig nyomj�k, addig tart ez a ciklus
		{
		s1++;
		if (s1 > 50)				// ha 4 sec eltelt, pittyeg�nk
			{					// s1 tulcsordulhat (25 sec-ig nyomva tart�skor, de nem baj
			beep(ok);				// pitty
			SFbits.Clear_Credit = 1;	// T�r�lni kell a krediteket
			} else
			{
			} // end of if
		Delayms(100);		//  100 ms delay * 50 ciklus = 5 sec kell a kredit törléshez
		s2 = KIBE_Pressed();		// várjuk, hogy elengedj�k a ki-be gombot
		} // end of while

// ki-be gomb kikerül�s v�ge

			pctap_be();		// mehet a PC t�p
			Delayms(1000);	// várunk

			atxst_pulse();		// 3 impulzust adunk ra
			Delayms(500);	// várunk, hogy �ledjen a t�p

		s1 = PC12V();

		while (!s1)							// amig nem �led fel a t�p, itt kering
			{
			beep(error);
			atxst_pulse();		// újra 3 impulzust adunk
			Delayms(500);	// várunk, hogy �ledjen a t�p

			s1 = PC12V();
			}

		mute_be();		// mute legyen
			//	montap_be
		run_led_on();		//  Start LED bekapcsolósa (ON állapot)


// A soros v�teli pufferben lévő üzenetek eldob�sa

		SFbits.In_Msg = 0;				// =1 ha �pp van bejövő üzenet
		RX_Msg = 0;
		rxiptr = rxoptr = 0;
	
// nyomogombok induláskori beragad�s�t ellenőrző r�sz

		s1 = PageLeft_Pressed();
		if (s1)									// ha nyomva van, újra r�olvasunk
			{
			Delayms(30);							// kis késleltetés újraolvasás el�tt
			s1 = PageLeft_Pressed();
			if (s1)									// 
				{
				BadButBits.Page_Left = 1;			// könyveljük, hogy beragadt
				}
			}

		s1 = PageRight_Pressed();
		if (s1)									// ha nyomva van, újra r�olvasunk
			{
			Delayms(30);							// kis késleltetés újraolvasás el�tt
			s1 = PageRight_Pressed();
			if (s1)									// 
				{
				BadButBits.Page_Right = 1;			// könyveljük, hogy beragadt
				}
			}

		s1 = StepLeft_Pressed();
		if (s1)									// ha nyomva van, újra r�olvasunk
			{
			Delayms(30);							// kis késleltetés újraolvasás el�tt
			s1 = StepLeft_Pressed();
			if (s1)									// 
				{
				BadButBits.Step_Left = 1;			// könyveljük, hogy beragadt
				}
			}


		s1 = StepRight_Pressed();
		if (s1)									// ha nyomva van, újra r�olvasunk
			{
			Delayms(30);							// kis késleltetés újraolvasás el�tt
			s1 = StepRight_Pressed();
			if (s1)									// 
				{
				BadButBits.Step_Right = 1;			// könyveljük, hogy beragadt
				}
			}

		s1 = Enter_Pressed();
		if (s1)									// ha nyomva van, újra r�olvasunk
			{
			Delayms(30);							// kis késleltetés újraolvasás el�tt
			s1 = Enter_Pressed();
			if (s1)								// 
				{
				BadButBits.Enter = 1;				// könyveljük, hogy beragadt
				}
			}


		s1 = Return_Pressed();
		if (s1)									// ha nyomva van, újra r�olvasunk
			{
			Delayms(30);							// kis késleltetés újraolvasás el�tt
			s1 = Return_Pressed();
			if (s1)								// 
				{
				BadButBits.Return = 1;				// könyveljük, hogy beragadt
				}
			}

		if(BadButBits.x != 64)					// beragadt gombn�l hibajelzés
			{
			beep(error);
			Delayms(200);
			beep(error);
			}

		// DIP kapcsoló beállít�sok beolvasása
		SFbits.Keyboard_Enable = DIP_Keyboard();		// =1 legyen, ha használhat� a bill., 0, ha nem
		SFbits.Long_Switchoff = DIP_Longoff();			// =1, ha hosszabb id� múlva kapcsoljuk ki a PC-t (XP)
		SFbits.Monitor_Always_On = DIP_Monitor();		// =1, ha a monitort azonnal be kell kapcsolni
		SFbits.Remote_2ch = DIP_2ch_Remote();		// =1, ha kétcsatornás távvezérlő van a géphez

	if (SFbits.Monitor_Always_On)
		{
		montap_be();							// DIP8 - a monitor kapcsolodjon be
		Delayms(400);						// monitor bekapcso után biztons�gi v�rakoz�s
		}


	s1 = CHK_eeprom_read(EEChkAddr);		// beolvassuk az EEPROM-b�l, hogy először fut-e a chip
		if(s1 == 0xAA)
			{
			BUTbits.Chip_First_Run = 1;		// =1, először fut a  a chip
			}

	// várunk a PC bejelentkezés�re
	// ha nem jelentkezik be x percig, bekapcsuljuk a monitort

		Delayms(500);				// várjuk, hogy biztosan fel�ledjen a tápegység
		Remote_Present();			// megnézzük, hogy van-e távvezérlő

	RM_Count = 0;					// ez m�ri majd a 3 percet --> monitor bekapcsolós
	s2 = 0;					// villogtat�s ciklusváltoz�ja

while(!TVbits.PC_Logged)		// Akkor tekintj�k bejelentkezettnek a PC-t ha a SETUP-ot és a számlálókat �tadtuk
	{
	while(!RX_Msg)			// v�r egy bejövő üzenetet
		{
		Delayms(13);
		s2++;
		RM_Count++;
		if(s2 < 40)
			{
			erme_lam_on();
			sleep_led_on();
			run_led_off();
			kibe_lam_on();
			} else
			{
			sleep_led_off();
			run_led_on();
			erme_lam_off();
			kibe_lam_off();
			}
		if (s2 == 80)
			{
			s2 = 0;
			if(!TVbits.PC_On)
				{
				beep(ok);						// pittyeg�nk, amig nem kapcsol be a PC
				}
			}						// kezdj�k el�lr�l a villogtat� ciklust

		SFbits.Monitor_Always_On = DIP_Monitor();		// megn�zi, kell-e monitort bekapcsolni
		if ((SFbits.Monitor_Always_On) || (RM_Count > 12000 ))	// ez kb. 3 perc
			{
			montap_be();							// DIP8 - ra azonnali monitor bekapcsolós
//			Delayms(500);
			}
	
	}

	if (RX_Msg)					// message handler 1
	{
	rxoptr++;						// STX átlépése
	rxoptr &= SER_FIFO_MASK;		// körbefordulás kezelése
	Cmd = rxfifo[rxoptr];			// a puffer következő karaktere a parancs
	rxoptr++;						// STX átlépése
	rxoptr &= SER_FIFO_MASK;		// körbefordulás kezelése

	switch(Cmd) {
		case m_setup_send : // bejövő setup adathalmaz
		{
		Get_Setup();
		rxoptr++;						// ETX átlépése
		rxoptr &= SER_FIFO_MASK;		// körbefordulás kezelése
		Dec_RX_Msg();					// üzenet kezelve
		Delayms(20);					// k�ldt�nk egy nyugt�t, hadd kezelje le a PC
		send_mess(m_counters_rq);		// számláló adatokat is k�r�nk
		}
		break;

		case m_counters_send : // bejövő számláló állások
		{
		Get_Counters();
//		send_mess(m_counters_rq);		// számláló adatokat is k�r�nk
		TVbits.PC_Logged = 1;	// A PC bejelentkezett, a számlálókat és a SETUP-ot �tadta/�tvette
		}
		break;

//		case m_setup_rq : // bejövő kérés setup küldés�re (nem lesz ilyen a PC fel�l)
//		{
//		Send_Setup();		// Setup adathalmaz küldése
//		}
//		break;
	
		case m_login : // A 'i'	PC bejelentkez� üzenete
		{
		rxoptr++;							// ETX átlépése
		rxoptr &= SER_FIFO_MASK;			// körbefordulás kezelése
		Dec_RX_Msg();					// üzenet kezelve
		montap_be();						// monitort bekapcsoljuk
		Delayms(400);
		Send_HW_Init();					// bejelentkezésre k�ldj�k a init-et
		amptap_be();
		TVbits.PC_On = 1;					// bejelentkezett a PC, kezdodik az adatcsere
		}
		break;

		case 'R' : // A PC jelzi az init üzenet után, hogy nem akar BIOS-t t�lteni
			{	
			rxoptr++;							// ETX átlépése
			rxoptr &= SER_FIFO_MASK;			// körbefordulás kezelése
			Dec_RX_Msg();					// üzenet kezelve
			mute_ki();
			if (BUTbits.Chip_First_Run)	// ha először fut a chip, SETUP-ot is k�r
				{
				send_mess(m_setup_rq);
				}	else
				{
				Send_Setup();		//  ha nem először fut, akkor k�ldi a SETUP-ot
			//	Send_Counters();	// a számláló állásokat is k�ldj�k
				TVbits.PC_Logged = 1;	// A PC bejelentkezett, a számlálókat és a SETUP-ot �tadta/�tvette
				}
			}
		break;

		case 'r' : // A PC jelzi az init üzenet után, hogy BIOS-t akar t�lteni
			{	
			rxoptr++;							// ETX átlépése
			rxoptr &= SER_FIFO_MASK;			// körbefordulás kezelése
			Dec_RX_Msg();					// üzenet kezelve
			// mute_ki();
			DownloadCode();					// BIOS friss�t� rutin
			}
		break;

		case m_mute_on : // 'u' - A PC k�relme MUTE bekapcsolósra
		{
		rxoptr++;							// ETX átlépése
		rxoptr &= SER_FIFO_MASK;			// körbefordulás kezelése
		Dec_RX_Msg();						// üzenet kezelve
		mute_be();
		putch(STX);
		putch(ACK);
		putch(ETX);
		}
		break;

		case m_mute_off : // 'U' A PC k�relme MUTE kikapcsolósra
		{
		rxoptr++;							// ETX átlépése
		rxoptr &= SER_FIFO_MASK;			// körbefordulás kezelése
		Dec_RX_Msg();						// üzenet kezelve
		mute_ki();
		putch(STX);
		putch(ACK);
		putch(ETX);
		}
		break;

		case m_counters_rq : // 'g' - kérelem számláló állások küldés�re a PC fel�l 
			{
			Send_Counters();
			rxoptr++;							// ETX átlépése
			rxoptr &= SER_FIFO_MASK;			// körbefordulás kezelése
			Dec_RX_Msg();						// üzenet kezelve
			Wait_For_Ack();			// megvárjuk a nyugt�t
			Delayms(10);				// késleltetés
			}	// end of case
			break;


	 	default:   							// a m�shol fel nem dolgozott üzenetek
 		{								 
			while (rxfifo[rxoptr] != ETX)	
				{
				 rxoptr++;
				 rxoptr &= SER_FIFO_MASK;		// megkeress�k az üzenet v�g�t
				}		
				 rxoptr++;
				 rxoptr &= SER_FIFO_MASK;		// ETX átlépése
				Dec_RX_Msg();					// üzenet kezelve
  
			putch(STX);				// nem �rtelmezett üzenet
			putch(NAK);
			putch(ETX);

 	 	}
		break;



		} // end of switch

		if(rxiptr < rxoptr)	// várjuk, hogy elromlik-e a mutat�
			{
			s1 = s2;
			}

		if(ForcedMuteOff)		// ha szakadt üzenet j�tt be, a mute-t kikapcsoljuk
			{
			mute_ki();
			ForcedMuteOff = 0;
			}

	} // end of MessageHandler 1 if

} // end of while(!TVbits.PC_Logged)


	if(BUTbits.Chip_First_Run)
		{
		eeprom_irhato = 1;
		CHK_eeprom_write(EEChkAddr, 0x55);		// EEPROM-ba �rjuk, hogy inicializ�ltuk a chipet
		eeprom_irhato = 0;
		BUTbits.Chip_First_Run = 0;		// jelezzük, hogy m�r nem először fut a chip
		}


	EEPROM_CH_Read();				// érmecsatorna - pénzérték t�bl�zat bet�lt�se
	EEPROM_Counters_Read();			// számláló állások bet�lt�se

	ermtilt_ki();		// Elektromos érmevizsgáló engedélyezése

	sleep_led_off();		// G�p elindult
	run_led_on();
	erme_lam_on();



while(1)
	{
	while(!RX_Msg)		// v�r egy bejövő üzenetet
		{
		Scan_Inputs();
		SFbits.Monitor_Always_On = DIP_Monitor();		// megn�zi, kell-e monitort bekapcsolni
		if (SFbits.Monitor_Always_On)
			{
			montap_be();							// DIP8 - ra azonnali monitor bekapcsolós
//			Delayms(500);
			}
	//	Counter.Hosszu++;		// n�velj�k a számlálókat, hogy mozg�st l�ssunk
	//	Counter.Rovid++;
		}

// message handler

	if (RX_Msg)

	{

	rxoptr++;					// STX átlépése
	rxoptr &= SER_FIFO_MASK;		// körbefordulás kezelése
	Cmd = rxfifo[rxoptr];			// a puffer következő karaktere a parancs
	rxoptr++;						// STX átlépése
	rxoptr &= SER_FIFO_MASK;		// körbefordulás kezelése

	switch(Cmd) {
		case m_setup_send : // bejövő setup adathalmaz
		{
		Get_Setup();
		// ETX átlépése kezelve a rutinban
		// az ch-érme ésszerendel�s t�mbj�t újrat�lteni
		rxoptr++;						// ETX átlépése
		rxoptr &= SER_FIFO_MASK;		// körbefordulás kezelése
		Dec_RX_Msg();				// üzenet kezelve
		EEPROM_CH_Read();			// érmecsatorna - pénzérték t�bl�zat bet�lt�se

		}
		break;

//		case m_setup_rq : // bejövő kérés setup küldés�re (nem lesz ilyen a PC fel�l)
//		{
//		Send_Setup();		// Setup adathalmaz küldése
//		}
//		break;
	
//		case m_login : // A PC bejelentkez� üzenete		// El�z� messagehandler kezeli
//		{
//		montap_be();		// monitort bekapcsoljuk
//		Send_HW_Init();	// bejelentkezésre k�ldj�k a init-et
//		amptap_be();
//		}
//		break;

//		case 'G' : // számláló állások a PC fel�l - ASCII-b�l kell dek�dolni, és EEPROM-ba �rni
//			{					// Ez csak a chip első futásakor fordul el�
//			Get_Counters();
//			}
//		break;

		case m_counters_rq : // 'g' - kérelem számláló állások küldés�re a PC fel�l 
			{
			Send_Counters();
			rxoptr++;							// ETX átlépése
			rxoptr &= SER_FIFO_MASK;			// körbefordulás kezelése
			Dec_RX_Msg();						// üzenet kezelve
			Wait_For_Ack();			// megvárjuk a nyugt�t
			Delayms(10);				// késleltetés
			}	// end of case
			break;


//		case 'r' : // BIOS t�lt�si sz�nd�k üzenete a PC fel�l
//		{
		// Send_Mess2(22);
//		}
//		break;

//		case 'R' : // A PC jelzi az init üzenet után, hogy nem akar BIOS-t t�lteni
//			{	
//			mute_ki();
//			Send_Setup();		// Setup adathalmaz küldése
//			}
//		break;

		case m_login : // A 'i'	PC bejelentkez� üzenete
		{
		rxoptr++;							// ETX átlépése
		rxoptr &= SER_FIFO_MASK;			// körbefordulás kezelése
		Dec_RX_Msg();					// üzenet kezelve
//		montap_be();						// monitort bekapcsoljuk
		Send_HW_Init();					// bejelentkezésre k�ldj�k a init-et
//		amptap_be();
//		TVbits.PC_On = 1;					// bejelentkezett a PC, kezdodik az adatcsere
		}
		break;

		case m_mute_on : // 'u' - A PC k�relme MUTE bekapcsolósra
		{
		rxoptr++;							// ETX átlépése
		rxoptr &= SER_FIFO_MASK;			// körbefordulás kezelése
		Dec_RX_Msg();						// üzenet kezelve
		mute_be();
		putch(STX);
		putch(ACK);
		putch(ETX);
		}
		break;

		case m_mute_off : // 'U' A PC k�relme MUTE kikapcsolósra
		{
		rxoptr++;							// ETX átlépése
		rxoptr &= SER_FIFO_MASK;			// körbefordulás kezelése
		Dec_RX_Msg();						// üzenet kezelve
		mute_ki();
		putch(STX);
		putch(ACK);
		putch(ETX);
		}
		break;

		case m_test_on : // 'X' - A PC jelzi, hogy Teszt módba lépett
		{
		rxoptr++;							// ETX átlépése
		rxoptr &= SER_FIFO_MASK;			// körbefordulás kezelése
		Dec_RX_Msg();						// üzenet kezelve
		TVbits.PC_Test_Mode = 1;
		putch(STX);
		putch(ACK);
		putch(ETX);
		}
		break;
	
		case m_test_off : // 'x' - A PC jelzi, hogy kilépett a TESZT módb�l
		{
		rxoptr++;							// ETX átlépése
		rxoptr &= SER_FIFO_MASK;			// körbefordulás kezelése
		Dec_RX_Msg();						// üzenet kezelve
		TVbits.PC_Test_Mode = 0;
		putch(STX);
		putch(ACK);
		putch(ETX);
		}
		break;

		case m_booking_send : // 'Z' - A PC rövid számláló null�z�si (zárási) k�relmet és a mai dátumot k�ldi
				{					// A HW a következő szabad zárási cimre kiirja az adatot.
				Get_Booking();
				// a rövid számlálót null�zni, kiirni!
				// ETX átlépés kezelve a rutinban
				}
				break;

		case m_booking_rq : // 'z' - A PC null�z�si (zárási) adatokat k�r
			{
			rxoptr++;						// �tl�pj�k az ETX-et
			rxoptr &= SER_FIFO_MASK;		// körbefordulás kezelése
			Dec_RX_Msg();				// üzenet kezelve
			Send_Booking();
			}	// end of send booking
		break;

		case ACK : // a PC norm�l nyugt�t k�ld�tt
		{
		rxoptr++;						// �tl�pj�k az ETX-et
		rxoptr &= SER_FIFO_MASK;		// körbefordulás kezelése
		Dec_RX_Msg();					// üzenet kezelve
		}
		break;

		case NAK : // a PC negativ nyugt�t k�ld�tt
		{
		rxoptr++;						// �tl�pj�k az ETX-et
		rxoptr &= SER_FIFO_MASK;		// körbefordulás kezelése
		Dec_RX_Msg();					// üzenet kezelve
		}
		break;

		case m_counter_reset: // 'n'kérés számlálók null�z�s�ra
				{
				Counter_Reset();
				// a rövid számlálót null�zni, kiirni!
				}
				break;



		case m_remote_credit : // '#' - Távvezérlővel beadtott kredit
		{
		rxoptr++;							// ETX átlépése
		rxoptr &= SER_FIFO_MASK;			// körbefordulás kezelése
		Dec_RX_Msg();					// üzenet kezelve
		beep(ok);
		putch(STX);
		putch(ACK);
		putch(ETX);
		}
		break;




		case m_shutdown : // 'Q' - A PC elfogadta a kikapcsolósi k�relmet
		{
		Proc_Shutdown();		// kiirjuk a krediteket és a hanger�t, utána kikapcs
		asm("nop");
		asm("nop");
		asm("nop");
		asm("BCF _PCLATH,4");
		asm("BCF _PCLATH,3");
		asm("GOTO 0x0000");		// restart, a HW újraindul
		}
		break;




	 	default:   							// a m�shol fel nem dolgozott üzenetek
 		{								 
			while (rxfifo[rxoptr] != ETX)	
				{
				 rxoptr++;
				 rxoptr &= SER_FIFO_MASK;		// megkeress�k az üzenet v�g�t
				}		
				 rxoptr++;
				 rxoptr &= SER_FIFO_MASK;		// ETX átlépése
				Dec_RX_Msg();					// üzenet kezelve
  
			putch(STX);				// nem �rtelmezett üzenet
			putch(NAK);
			putch(ETX);

 	 	}
		break;
	} // end of switch

		if(ForcedMuteOff)		// ha szakadt üzenet j�tt be, a mute-t kikapcsoljuk
			{
			mute_ki();
			ForcedMuteOff = 0;
			}
		

		if(rxiptr < rxoptr)	// várjuk, hogy elromlik-e a mutat�
			{
			s1 = s2;
			}

//		SFbits.RX_Msg = 0;	// üzenet kezelve, üzenetjelz� bit törlése
//		rxiptr = 0;				// soros v�teli puffer mutat� alaphelyzetbe
		} // end of MessageHandler if


	} // end of while(1)


}  // end of main

/**********************************************************************
* INTEL HEX form�tum� k�d let�lt�se
***********************************************************************/

 // #define	Z	2	// STATUS reg bit2 zero bit
 // #define	C	0	// STATUS reg bit0 carry bit
 // #define	RP0	5	// STATUS reg bit0 carry bit

// regiszter muveletek celregisztere
 // #define	F	1	// a muvelet eredmenye a FileReg-be menjen
 // #define	W	0	// a muvelet eredmenye a WREG-be menjen

// #define	__16F876A

// #pragma psect standardPsect=DCode

// #pragma psect text%%u=DownloadCodefunc

 void	DownloadCode(void)	// @ 0x1500		//	Uses USART to receive data from PC
{
	OCL1 = 1;
	OCL1 = 0;
	OCL1 = 1;


/*

#asm
//	#include 	"aspic.h"
	#include 	"as168xa.h"

		banksel	_PORTC			; RCREG
DCStart:
		call	GetByte			// várunk egy karaktert a soros porton
		movlw	':'			// kett�spontot várunk, ha nem az jön, v�r a ciklus
		subwf	_PORTC,w
		nop
		nop
		nop
//		btfss	_STATUS,Z
		btfss	ZERO
							;		btfss	_ZERO
		goto	DCStart

		call	GetHex8				// Read byte count
		movwf	_bnum		// t�roljuk
		movwf	_local_CLC		// ez m�r r�sze lesz a checksum-nak is
//		bcf	STATUS,C
		bcf	CARRY
//		btfss	ZERO
		rrf	_bnum,f			// felezzük, ennyi sz�t (word) fogunk venni

		call	GetHex8				// A 16 bites cím felső b�jtja
		movwf	_AddrH
		addwf	_local_CLC,f	// minden megy a checksum-ba is
		call	GetHex8				// A 16 bites cím als� b�jtja
		movwf	_AddrL
		addwf	_local_CLC,f	// ez is a checksumba

		call	GetHex8				// rekord tipus
		movwf	_RecType
		addwf	_local_CLC,f	// checksum

DataRec:							// adatok v�tele
		movf	_RecType,f		// adatrekord ellenőrzős  (0h a j� érték)
//		btfss	STATUS,Z
		btfss	ZERO
		goto		EndOfFileRec			// ha nem 0, akkor EOF ellenőrzős (01h)

DRLoop:	
		movf	_bnum,f		// bytecount = 0 ? akkor nem jön több adat
//		btfsc		_STATUS,Z
		btfsc		ZERO
		goto		DRCkChecksum		// ha 0, akkor checksum ellenőrzős
		call		GetHex8				// adat als� byte-ja, 2 ASCII karakter
		movwf	_HexDataL		// checksumba
		addwf	_local_CLC,f
		call		GetHex8				// adat felső b�jtja, 2 ASCII karakter
		movwf	_HexDataH		// checksumba
		addwf	_local_CLC,f

WriteDataSequence:				// a kapott adatot a FLASH-be �rjuk
		banksel	_EEADRH
		movf	_AddrH,w			// Write address to EEADRH:EEADR registers
		movwf	_EEADRH
		movf	_AddrL,w
		movwf	_EEADRL
		movf	_HexDataH,w		// Write data to EEDATH:EEDATA registers
		movwf	_EEDATH
		movf	_HexDataL,w
		movwf	_EEDATA
		banksel	_EECON1			// Write sequence
		bsf	EEPGD		// Set EEPGD to indicate program memory
//		bsf	_EECON1,_WREN		// Enable writes to memory
		bsf		WREN		// Enable writes to memory
//		bcf	_INTCON,_GIE			// Make sure interrupts are disabled
		bcf		GIE			// Make sure interrupts are disabled
		movlw	0x55			// Required write sequence
		movwf	_EECON2
		movlw	0xaa
		movwf	_EECON2
//		bsf	_EECON1,wr			// Start internal write cycle
		bsf	WR				// Start internal write cycle
		nop
		nop
	//	bcf	_EECON1,WREN		// Disable writes
		bcf		WREN		// Disable writes

		banksel	_EECON1			// Read sequence
//		bsf	_EECON1,_EEPGD		// Set EEPGD to indicate program memory
		bsf		EEPGD		// Set EEPGD to indicate program memory
//		bsf	_EECON1,RD			// Enable reads from memory
		bsf	RD			// Enable reads from memory

//		bcf	_STATUS,RP0
		bcf	RP0
		nop
		movf	_EEDATH,w		// Compare memory value to HexDataH:HexDataL
		subwf	_HexDataH,w
//		btfss	_STATUS,Z
		btfss	ZERO
		retlw	0xff					// If upper byte not equal, return FFh
		movf	_EEDATA,w		// to indicate programming failure
		subwf	_HexDataL,w
//		btfss	_STATUS,Z
		btfss	ZERO
		retlw	0xff					// If lower byte not equal, return FFh
								// to indicate programming failure
		incf	_AddrL,f				// Increment address for next iteration
//		btfsc		_STATUS,Z
		btfsc		ZERO
		incf	_AddrH,f
		decf	_bnum,f			// Decrement byte count
		goto	DRLoop				// Go back to check for ByteCount = 0

DRCkChecksum:					// Checksum verification
		call	GetHex8				// Read in checksum
		addwf	_local_CLC,w	// Add to calculated checksum
//		btfss	_STATUS,Z		// Result should be 0
		btfss	ZERO
		retlw	1					//  If not return 1 to indicate checksum fail
		goto	DCStart				// Do it again

EndOfFileRec:						// End of File record (01h)
		decf	_RecType,w			// If EOF record, decrement should = 0
//		btfss	_STATUS,Z
		btfss	ZERO
		goto	DCStart				// Not valid record type, wait for next :
		call	GetHex8				// Read in checksum
		addwf	_local_CLC,w	// Add to calculated checksum
//		btfss	_STATUS,Z		// Result should be 0
		btfss	ZERO
		retlw	1					// If not return 1 to indicate checksum fail
		retlw	0					// Otherwise return 0 to indicate success


GetByte:
	;clear CTS
;	banksel	PIR1
;	GH4Wait		btfss	PIR1,RCIF
;	goto		GH4Wait
	;set CTS
	nop
	banksel	_PORTC
	movf	_PORTC,w
	return

GetHex8:						// This function uses the USART
		call		GetByte			// Read a character from the USART
		call		ASCII2Hex		// Convert the character to binary
		movwf	_Temp			// Store result in high nibble
		swapf	_Temp,f

		call		GetByte			// Read a character from the USART
		call		ASCII2Hex		// Convert the character to binary
		iorwf		_Temp,f			// Store result in low nibble
		movf	_Temp,w			// Move result into WREG
		return

ASCII2Hex:						// Convert value to binary
		movwf	_Temp1			// Subtract ASCII 0 from number
		movlw	'0'
		subwf	_Temp1,f
		movlw	0xf0				// If number is 0-9 result, upper nibble
		andwf	_Temp1,w			// should be zero
//		btfsc		_STATUS,Z
		btfsc		ZERO
		goto		ASCIIOut
		movlw	'A'-'0'-0x0a		// Otherwise, number is A - F, so
		subwf	_Temp1,f			// subtract off additional amount
ASCIIOut:
		movf	_Temp1,w			// Value should be 0 - 15
		return

#endasm


*/

} // end of downloader


void	kamufuggveny(void)	//	Uses USART to receive data from PC
{

s1 = (s1 + 100) - 100;

}



