/*
 * File:   myModule.c
 * Author: Ian Hogg
 * 
 * Example main file for a module.
 *
 * Created on 10 April 2017, 10:26
 */


#include <xc.h>

#include "module.h"
#include "StatusLeds.h"
#include "events.h"
#include "FLiM.h"
#include "romops.h"

unsigned char canid = 0;        // initialised from ee
unsigned int nn = DEFAULT_NN;   // initialised from ee

extern BYTE BlinkLED( BOOL blinkstatus );

// local function prototypes
/*
 */
void __init(void);
BOOL checkCBUS( void);
void ISRHigh(void);
void initialise(void);
void configIO(unsigned char io);
void defaultPersistentMemory(void);
void setType(unsigned char i, unsigned char type);
void setOutput(unsigned char i, unsigned char state, unsigned char type);

#ifdef __C18__
void high_irq_errata_fix(void);

/*
 * Interrupt vectors (moved higher when bootloader present)
 */

// High priority interrupt vector

//#ifdef BOOTLOADER_PRESENT
    #pragma code high_vector=0x808
//#else
//    #pragma code high_vector=0x08
//#endif


//void interrupt_at_high_vector(void)

void HIGH_INT_VECT(void)
{
    _asm
        CALL high_irq_errata_fix, 1
    _endasm
}

/*
 * See 18F2480 errata
 */
void high_irq_errata_fix(void) {
    _asm
        POP
        GOTO ISRHigh
    _endasm
}

// low priority interrupt vector

//#ifdef BOOTLOADER_PRESENT
    #pragma code low_vector=0x818
//#else
//    #pragma code low_vector=0x18
//#endif

void LOW_INT_VECT(void)
{
//    _asm GOTO ISRLow _endasm
}
#endif

// MAIN APPLICATION
        
/**
 * It is all run from here.
 * Initialise everything and then loop receiving and processing CAN messages.
 */
#ifdef __C18__
void main(void) {
#else
int main(void) @0x800 {
#endif
    TickValue   startTime;
    BOOL        started = FALSE;
  
    initialise();
    startTime.Val = tickGet();
 
    while (TRUE) {
        // Startup delay for CBUS about 2 seconds to let other modules get powered up - ISR will be running so incoming packets processed
        if (!started && (tickTimeSince(startTime) > (NV->sendSodDelay * HUNDRED_MILI_SECOND) + TWO_SECOND)) {
            started = TRUE;
            if (NV->sendSodDelay > 0) {
                sendStartupSod(SOD_PRODUCED_ACTION);
            }
        }
        checkCBUS();    // Consume any CBUS message - display it if not display message mode
        FLiMSWCheck();  // Check FLiM switch for any mode changes
        // Module specific stuff here
        // Check for any flashing status LEDs
        checkFlashing();
     } // main loop
} // main
 

/**
 * The order of initialisation is important.
 */
void initialise(void) {
        
    // check if EEPROM is valid
    if (ee_read((WORD)EE_RESET) != 0xCA) {
        // set EEPROM and Flash to default values
        defaultPersistentMemory();
        // set the reset flag to indicate it has been initialised
        ee_write((WORD)EE_RESET, 0xCA);
    }
    canid = ee_read((WORD)EE_CAN_ID);
    nn = ee_read((WORD)EE_NODE_ID);
    
    flimInit(); // Maybe also call module specific init 


    // enable interrupts, all init now done
    ei();
 
    setStatusLed(flimState == fsFLiM);
}    

/**
 * Set up the EEPROM and Flash.
 * Should only get called once on first power up. Initialised EEPROM and Flash.
 */
void defaultPersistentMemory(void) {
    // set EEPROM to default values
    ee_write((WORD)EE_BOOT_FLAG, 0);
    ee_write((WORD)EE_CAN_ID, DEFAULT_CANID);
    ee_write_short((WORD)EE_NODE_ID, DEFAULT_NN); 
    ee_write((WORD)EE_FLIM_MODE, fsSLiM);
}

/**
 * Check to see if a message has been received on the CBUS and process 
 * it if one has been received.
 * @return true if a message has been received.
 */
BOOL checkCBUS( void ) {
    char    msg[20];

    if (cbusMsgReceived( 0, (BYTE *)msg )) {
        LED2G = BlinkLED( 1 );           // Blink LED on whilst processing messages - to give indication how busy module is
        parseCBUSMsg(msg);               // Process the incoming message
        return TRUE;
    }
    return FALSE;
}

/**
 * Handle the Consumed event.
 */
void processEvent(unsigned char action, BYTE * msg) {
    
}

/**
 * Validate the the NV change is OK.
 * @param NVindex
 * @param NVvalue
 * @return true if OK
 */
BOOL validateNV(BYTE NVindex, BYTE oldvalue, BYTE newValue) {
    return TRUE;
}

/**
 * Ensure any changes to NVs take effect.
 * @param NVindex
 * @param NVvalue
 */
void actUponNVchange(BYTE NVindex, BYTE NVvalue) {
    
}


#ifdef __C18__
// C intialisation - declare a copy here so the library version is not used as it may link down in bootloader area

void __init(void)
{
}

// Interrupt service routines
#if defined(__18CXX)
    #pragma interruptlow ISRHigh
    void ISRHigh(void)
#elif defined(__dsPIC30F__) || defined(__dsPIC33F__) || defined(__PIC24F__) || defined(__PIC24FK__) || defined(__PIC24H__)
    void _ISRFAST __attribute__((interrupt, auto_psv)) _INT1Interrupt(void)
#elif defined(__PIC32MX__)
    void __ISR(_EXTERNAL_1_VECTOR, ipl4) _INT1Interrupt(void)
#else
    void _ISRFAST _INT1Interrupt(void) {
#endif
#else 
    void interrupt low_priority low_isr(void) {
#endif
    tickISR();
    canInterruptHandler();
}

void interrupt high_priority high_isr (void)
{
 /* service routine body goes here */
}


