/* 
 * File:   module.h
 * Author: Ian
 *
 * The CBUS library files include this module specific header. 
 * This is the means to isolate dependencies between the library and the module
 * specific code. All the dependencies of the library on the module specific code
 * should be defined in this file.
 * 
 * In particular EEPROM, NV and Event definitions should be here or included from here.
 *  
 * Created on 15 April 2017, 21:33
 */

#ifndef MODULE_H
#define	MODULE_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    
/*
 * BOOTLOADER
 */
#define BOOTLOADER_PRESENT


/*
 * NVs
 */


/*
 * This structure is required by FLiM.h
 */
typedef struct {
	// fill in your NVs here
} ModuleNvDefs;


#define NV_NUM  sizeof(ModuleNvDefs)     // Number of node variables
#define AT_NV   0x7F80                  // Where the NVs are stored. (_ROMSIZE - 128)  Size=128 bytes



/*
 * EVENTS
 */

#define NUM_PRODUCER_ACTIONS                0	// CHANGE THIS
#define NUM_CONSUMER_ACTIONS                0	// CHANGE THIS
  
#define NUM_ACTIONS                         (NUM_CONSUMER_ACTIONS + NUM_PRODUCER_ACTIONS)

// These are chosen so we don't use too much memory 32*20 = 640 bytes.
// Used to size the hash table used to lookup events in the events2actions table.
#define HASH_LENGTH     32
#define CHAIN_LENGTH    20

#define EVT_NUM                 NUM_ACTIONS // Number of events
#define EVperEVT                17          // Event variables per event - just the action
#define NUM_CONSUMED_EVENTS     192         // number of events that can be taught
#define AT_ACTION2EVENT         0x7E80      //(AT_NV - sizeof(Event)*NUM_PRODUCER_ACTIONS) Size=256 bytes
#define AT_EVENT2ACTION         0x6E80      //(AT_ACTION2EVENT - sizeof(Event2Action)*HASH_LENGTH) Size=4096bytes


#ifdef	__cplusplus
}
#endif

#endif	/* MODULE_H */

