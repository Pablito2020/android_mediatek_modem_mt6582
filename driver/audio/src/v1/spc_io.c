/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2005
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

/*******************************************************************************
 *
 * Filename:
 * ---------
 * spc_io.c
 *
 * Project:
 * --------
 * MAUI
 *
 * Description:
 * ------------
 * IO interface between AP and MD speech driver
 *
 * Author:
 * -------
 * 
 *
 *==============================================================================
 *             HISTORY
 * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 *
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *==============================================================================
 *******************************************************************************/


#include "kal_public_defs.h"
#include "kal_public_api.h"
#include "kal_general_types.h"
#include "kal_trace.h"

#include "ccci.h"

#include "drv_sap.h"
#include "audio_msgid.h"

#include "audio_def.h"
#include "audio_enum.h"
#include "l1sp_trc.h"
#include "l1audio.h"
#include "spc_io.h"
#include "spc_drv.h"

// TODO:
// #define __CCCI_VIA_SDIO__ i.e. __SDIO_DEVICE_CONNECTIVITY__
extern uint32 SaveAndSetIRQMask( void );
extern void   RestoreIRQMask( uint32 );
extern uint32 L1I_GetTimeStamp( void );

// ----------------------------------------------------------------------------
// Definition using in SPC IO
// ----------------------------------------------------------------------------

#ifdef __SDIO_DEVICE_CONNECTIVITY__
#define SPCIO_CCCI_STREAM_BUF_RESERVED_SIZE 16 //unit is byte
#define SPCIO_SHARE_BUF_SIZE 8192
#endif //__SDIO_DEVICE_CONNECTIVITY__

#define SPCIO_CCCI_MSG_QUEUE_SIZE 32

typedef enum {
	SPCIO_QUEUE_STATUS_INIT = 0,
	SPCIO_QUEUE_STATUS_READY,
	
} SPCIO_QUEUE_STATUS_T;

// ----------------------------------------------------------------------------
// Structure using in SPC IO
// ----------------------------------------------------------------------------
typedef struct _SPCIO_MESSAGE_T_
{
	SPC_MSG_ID_T message_id; // messaage using in AUDL
	kal_uint32 message_data; // to put CCCI_data
	kal_uint32 message_dataRev; // to put CCCI_Reserved
	
	kal_uint32 putTime;
	kal_uint32 getTime;
} SPCIO_MESSAGE_T;

typedef struct _SPCIO_T_
{
   RINGBUFFER_T(SPCIO_MESSAGE_T, SPCIO_CCCI_MSG_QUEUE_SIZE)   ccciq;

	//ilm_struct *pIlm; 

	//share buffer related
	kal_uint32 apShareBufSize; // The size of AP share buffer, in size of Byte
   kal_uint32 mdShareBufSize; // The size of MD share buffer, in size of Byte (MD to AP)
   kal_uint32 apShareBufWrite;
	kal_uint32 mdShareBufWrite;
   kal_uint32 apShareBufRead;
	kal_uint32 mdShareBufRead;
	kal_uint8 *pApShareBuf;     // The base of AP share buffer
	kal_uint8 *pMdShareBuf;     // The base of MD share buffer (MD to AP)

	SPCIO_QUEUE_STATUS_T ccciqStatue; // to keep the queue status, make em_data_request is first process in the queue
	
   kal_bool isTaskInitialize;
} SPCIO_T;

// ----------------------------------------------------------------------------
// Global varialbe
// ----------------------------------------------------------------------------


SPCIO_T gSpcIo;


#ifdef __SDIO_DEVICE_CONNECTIVITY__
// [REMIND] Please use gSpcIo pointer intead of following buffer directly.

__attribute__  ((section ("NONCACHEDZI"))) kal_uint8 gSpcIo_MdShareBuffer[SPCIO_SHARE_BUF_SIZE];
__attribute__  ((section ("NONCACHEDZI"))) kal_uint8 gSpcIo_ApShareBuffer[SPCIO_SHARE_BUF_SIZE];
#endif //__SDIO_DEVICE_CONNECTIVITY__

// ----------------------------------------------------------------------------
// Logging
// ----------------------------------------------------------------------------

/**
	@prMsg: [input] pointer to structure SPCIO_MESSAGE_T. 
	@state: [input] logging state. 0 for receive, 1 for begin process, 2 for finish process
*/
static void _spcIO_MsgLog(SPCIO_MESSAGE_T *prMsg, kal_uint16 state)
{
   
   //kal_uint32 msgTypeAndFunc = SPCIO_CCCI_MSG_ORG_TYPE_AND_FUNC(prMsg->message_data);
	
   ASSERT( prMsg != NULL);

	// AUDL thread msg logging
	if(prMsg->message_id != SPC_ID_AUDIO_A2M_CCCI) {
		if(state == 2) {
         if(!kal_if_hisr())
            kal_trace(TRACE_INFO, SPCIO_FINISH_MODEM_INTERNAL, prMsg->message_id, prMsg->message_id);
         else
            kal_dev_trace(TRACE_INFO, SPCIO_FINISH_MODEM_INTERNAL, prMsg->message_id, prMsg->message_id);
      } else if(state == 1) {
         if(!kal_if_hisr())
            kal_trace(TRACE_INFO, SPCIO_PROCESS_MODEM_INTERNAL, prMsg->message_id, prMsg->message_id);
         else
            kal_dev_trace(TRACE_INFO, SPCIO_PROCESS_MODEM_INTERNAL, prMsg->message_id, prMsg->message_id);
      } else if(state == 0) {
         if(!kal_if_hisr())
            kal_trace(TRACE_INFO, SPCIO_RECEIVE_MODEM_INTERNAL, prMsg->message_id, prMsg->message_id);
         else
            kal_dev_trace(TRACE_INFO, SPCIO_RECEIVE_MODEM_INTERNAL, prMsg->message_id, prMsg->message_id);
      }
		return;
	}

	// CCCI message logging	 ========================================
	// original version for MAUI branch (like MT6573, MT6575, MT6577)
#ifdef SPC_MSG_ORG_VER		
	if(!(SPCIO_CCCI_MSG_IS_EXTEND(prMsg->message_data))) { 	
		AUD_CCCI_MSG_TYPE_T eMsgType;
	   eMsgType = (AUD_CCCI_MSG_TYPE_T)(SPCIO_CCCI_MSG_ORG_TYPE(prMsg->message_data));
	   
	   switch (eMsgType)
	   {
	      case MSG_TYPE_VOLUME_CTRL:
	      case MSG_TYPE_SET_DEVICE:
	      case MSG_TYPE_SPEECH_CTRL:
	      case MSG_TYPE_DATA_TRANSFER_CTRL:
	      case MSG_TYPE_OTHER_API_HANDLER:
	      case MSG_TYPE_ACK_FROM_AP:
	      {
	         kal_uint16 eFunc;
	         eFunc = (SPCIO_CCCI_MSG_ORG_FUNC1(prMsg->message_data));
	         //eFunc = (SPCIO_CCCI_MSG_ORG_FUNC(prMsg->message_data));
	         
				if(state == 2)
	         {
	            if(!kal_if_hisr())
	               kal_trace(TRACE_INFO, SPCIO_ORG_FINISH_COMMAND, prMsg->message_data, prMsg->message_id, prMsg->message_id, eMsgType, eMsgType, eFunc, eFunc + eMsgType * 0x10);
	            else
	               kal_dev_trace(TRACE_INFO, SPCIO_ORG_FINISH_COMMAND, prMsg->message_data, prMsg->message_id, prMsg->message_id, eMsgType, eMsgType, eFunc, eFunc + eMsgType * 0x10);
	         } else if(state == 1)
	         {
	            if(!kal_if_hisr())
	               kal_trace(TRACE_INFO, SPCIO_ORG_PROCESS_COMMAND, prMsg->message_data, prMsg->message_id, prMsg->message_id, eMsgType, eMsgType, eFunc, eFunc + eMsgType * 0x10);
	            else
	               kal_dev_trace(TRACE_INFO, SPCIO_ORG_PROCESS_COMMAND, prMsg->message_data, prMsg->message_id, prMsg->message_id, eMsgType, eMsgType, eFunc, eFunc + eMsgType * 0x10);
	         } else if(state == 0)
	         {
	            if(!kal_if_hisr())
	               kal_trace(TRACE_INFO, SPCIO_ORG_RECEIVE_COMMAND, prMsg->message_data, prMsg->message_id, prMsg->message_id, eMsgType, eMsgType, eFunc, eFunc + eMsgType * 0x10);
	            else
	               kal_dev_trace(TRACE_INFO, SPCIO_ORG_RECEIVE_COMMAND, prMsg->message_data, prMsg->message_id, prMsg->message_id, eMsgType, eMsgType, eFunc, eFunc + eMsgType * 0x10);
	         }
	         
	         break;
	      }
		  
	      case MSG_TYPE_NOTIFICATION:
			case MSG_TYPE_MISC_CTRL:
	      {
	         kal_uint16 eNotify;
	         kal_uint32 offset, length;
	         // offset   = (prMsg->message_data<<6)>>19;
	         // length   = (prMsg->message_data<<19)>>19;
	         offset = SPCIO_CCCI_MSG_ORG_PARAM2_OFFSET(prMsg->message_data);
				length = SPCIO_CCCI_MSG_ORG_PARAM2_LENGTH(prMsg->message_data);
	         eNotify  =  SPCIO_CCCI_MSG_ORG_TYPE_AND_FUNC2(prMsg->message_data);
	         //eNotify  =  SPCIO_CCCI_MSG_ORG_FUNC(prMsg->message_data);         
				
				if(state == 2)
	         {
	            if(!kal_if_hisr())
	               kal_trace(TRACE_INFO, SPCIO_ORG_FINISH_DATA_NOTIFY, prMsg->message_data, prMsg->message_id, prMsg->message_id, eMsgType, eMsgType, eNotify, eNotify, offset, length);
	            else
	               kal_dev_trace(TRACE_INFO, SPCIO_ORG_FINISH_DATA_NOTIFY, prMsg->message_data, prMsg->message_id, prMsg->message_id, eMsgType, eMsgType, eNotify, eNotify, offset, length);
				} else if(state == 1)
	         {
	            if(!kal_if_hisr())
	               kal_trace(TRACE_INFO, SPCIO_ORG_PROCESS_DATA_NOTIFY, prMsg->message_data, prMsg->message_id, prMsg->message_id, eMsgType, eMsgType, eNotify, eNotify, offset, length);
	            else
	               kal_dev_trace(TRACE_INFO, SPCIO_ORG_PROCESS_DATA_NOTIFY, prMsg->message_data, prMsg->message_id, prMsg->message_id, eMsgType, eMsgType, eNotify, eNotify, offset, length);
	         } else if(state == 0)
	         {
	            if(!kal_if_hisr())
	               kal_trace(TRACE_INFO, SPCIO_ORG_RECEIVE_DATA_NOTIFY, prMsg->message_data, prMsg->message_id, prMsg->message_id, eMsgType, eMsgType, eNotify, eNotify, offset, length);
	            else
	               kal_dev_trace(TRACE_INFO, SPCIO_ORG_RECEIVE_DATA_NOTIFY, prMsg->message_data, prMsg->message_id, prMsg->message_id, eMsgType, eMsgType, eNotify, eNotify, offset, length);
	         }
	      }
	      
	      break;
	      
	    default:
	        break;
	    }
		
		return;
	 }
#else	
	// new version for MOLY branch (Chip like MT6280)
	{
	kal_uint16 cmd = SPCIO_CCCI_MSG_CMD(prMsg->message_data);
	kal_uint16 data16 = SPCIO_CCCI_MSG_DATA16(prMsg->message_data);
	
	if(state == 2)	{
	   if(!kal_if_hisr())
	      kal_trace(TRACE_INFO, SPCIO_FINISH_MSG, prMsg->message_data, prMsg->message_id, prMsg->message_id, cmd, cmd, data16, prMsg->message_dataRev);
	   else
	      kal_dev_trace(TRACE_INFO, SPCIO_FINISH_MSG, prMsg->message_data, prMsg->message_id, prMsg->message_id, cmd, cmd, data16, prMsg->message_dataRev);
	
	} else if(state == 1) {
		if(!kal_if_hisr())
	      kal_trace(TRACE_INFO, SPCIO_PROCESS_MSG, prMsg->message_data, prMsg->message_id, prMsg->message_id, cmd, cmd, data16, prMsg->message_dataRev);
	   else
	      kal_dev_trace(TRACE_INFO, SPCIO_PROCESS_MSG, prMsg->message_data, prMsg->message_id, prMsg->message_id, cmd, cmd, data16, prMsg->message_dataRev);
	} else if(state == 0) {
	   if(!kal_if_hisr())
	      kal_trace(TRACE_INFO, SPCIO_RECEIVE_MSG, prMsg->message_data, prMsg->message_id, prMsg->message_id, cmd, cmd, data16, prMsg->message_dataRev);
	   else
	      kal_dev_trace(TRACE_INFO, SPCIO_RECEIVE_MSG, prMsg->message_data, prMsg->message_id, prMsg->message_id, cmd, cmd, data16, prMsg->message_dataRev);
	}
	}
#endif //#ifdef SPC_MSG_ORG_VER	
}

// ----------------------------------------------------------------------------
// Software Queue Related
// ----------------------------------------------------------------------------

extern ilm_struct *allocate_ilm(module_type module_id);

/**
	@prSmpMsg:
	@comeFrom: 0 for trigger from CCCI_HISR, 1 for trigger from AUDL. (you can add more in the future
*/
void SpcIO_MsgQueuePut(SPC_MSG_ID_T msgId, uint32 msgData, uint32 msgDataRev, kal_uint8 callerFrom)
{    
	kal_uint32 u4SavedMask;
	kal_uint32 isEmpty;
	SPCIO_MESSAGE_T queueMsg;

	queueMsg.message_id = msgId;
	queueMsg.message_data = msgData;
	queueMsg.message_dataRev = msgDataRev;
	
	//logging
	_spcIO_MsgLog(&queueMsg, 0);

	// Not Full
	if(gSpcIo.ccciq.write >= gSpcIo.ccciq.read) {
		ASSERT( (gSpcIo.ccciq.write - gSpcIo.ccciq.read) < SPCIO_CCCI_MSG_QUEUE_SIZE );
	} else {
		ASSERT( (gSpcIo.ccciq.write + (0xffffffff - gSpcIo.ccciq.read) +1 ) < SPCIO_CCCI_MSG_QUEUE_SIZE );
	}

	// disable inturrupt to prevent race condition, 
	// checking queue status and put data into sw queue
	u4SavedMask = SaveAndSetIRQMask();

	isEmpty = RB_EMPTY(gSpcIo.ccciq);	
	gSpcIo.ccciq.queue[gSpcIo.ccciq.write & (SPCIO_CCCI_MSG_QUEUE_SIZE-1)].message_id = msgId;
	gSpcIo.ccciq.queue[gSpcIo.ccciq.write & (SPCIO_CCCI_MSG_QUEUE_SIZE-1)].message_data = msgData;
	gSpcIo.ccciq.queue[gSpcIo.ccciq.write & (SPCIO_CCCI_MSG_QUEUE_SIZE-1)].message_dataRev = msgDataRev;
	gSpcIo.ccciq.queue[gSpcIo.ccciq.write & (SPCIO_CCCI_MSG_QUEUE_SIZE-1)].putTime = L1I_GetTimeStamp();
	gSpcIo.ccciq.write++;

	RestoreIRQMask(u4SavedMask);

   //prevent Modem side from processing A2M message before L1AudioMain is initialized, 
   //and just put A2M message into queue to wait the initilization of L1AudioMain      
	if(gSpcIo.isTaskInitialize != KAL_FALSE){
		// sned a taskMsg to AUDL to process when sw queue is empty then begin process. 
		if(isEmpty) {
			
			msg_send4(MOD_L1SP, MOD_MED, AUDIO_SAP, MSG_ID_AUDIO_A2M_CCCI);
		}
	}
    
}

void spcIO_MsgQueueGet(SPCIO_MESSAGE_T * prSmpMsg)
{
	kal_uint32 u4SavedMask;

	ASSERT( gSpcIo.ccciq.read != gSpcIo.ccciq.write );
	ASSERT( prSmpMsg != NULL);

	u4SavedMask = SaveAndSetIRQMask();

	prSmpMsg->message_id = gSpcIo.ccciq.queue[gSpcIo.ccciq.read & (SPCIO_CCCI_MSG_QUEUE_SIZE-1)].message_id;
	prSmpMsg->message_data = gSpcIo.ccciq.queue[gSpcIo.ccciq.read & (SPCIO_CCCI_MSG_QUEUE_SIZE-1)].message_data;
	prSmpMsg->message_dataRev = gSpcIo.ccciq.queue[gSpcIo.ccciq.read & (SPCIO_CCCI_MSG_QUEUE_SIZE-1)].message_dataRev;
	gSpcIo.ccciq.queue[gSpcIo.ccciq.read & (SPCIO_CCCI_MSG_QUEUE_SIZE-1)].getTime = L1I_GetTimeStamp();
	gSpcIo.ccciq.read++;

	RestoreIRQMask(u4SavedMask);
	
	_spcIO_MsgLog(prSmpMsg, 1);  

}

// ----------------------------------------------------------------------------
// Function related to CCCI
// ----------------------------------------------------------------------------

void SpcIO_A2M_hisr(CCCI_BUFF_T *bufp)
{
	kal_uint32 ccciMsg;

	/* confirm the channel buffer type, and get the mailbox id */
	if (CCCI_IS_MAILBOX(bufp)) {

		ccciMsg = CCCI_MAILBOX_ID(bufp);

		SpcIO_MsgQueuePut(SPC_ID_AUDIO_A2M_CCCI, ccciMsg, (bufp->reserved), 0);
		
	} else {
	
		// if data send via ccci stream, the following checking needs to remove.
#ifndef __SDIO_DEVICE_CONNECTIVITY__ 
		ASSERT(0);
#else	// CCCI via SDIO
		// process the data then put to internal share buffer. 
		kal_uint32 segment = 0;
		kal_uint32 offset;
		
		kal_uint32 dataLen;
		kal_uint8 *data;
	
		data =  (void *)CCCI_STREAM_ADDR(bufp);			
		dataLen = CCCI_STREAM_LEN(bufp);
		kal_prompt_trace(MOD_L1SP, "[1]A2M HISR bufp addr: %08x, len = %x", data, dataLen);

		if(dataLen>16) {
			dataLen = (dataLen-16);
			data = ((kal_uint8 *)bufp)+16;
		} else {
			dataLen = 0;
			data = NULL;
		}
		//do memory copy, and send message to queue
		
		kal_prompt_trace(MOD_L1SP, "[2]A2M HISR bufp addr: %08x, len = %x", data, dataLen);
		
		offset = gSpcIo.apShareBufWrite;
		
		if(dataLen>gSpcIo.apShareBufSize){ // illegle
			ASSERT(0);
		} else {
			segment = gSpcIo.apShareBufSize - offset;
			if(segment>=dataLen){
				segment = dataLen;
				kal_mem_cpy( (gSpcIo.pApShareBuf)+offset, data, segment*sizeof(uint8));
				gSpcIo.apShareBufWrite += segment;

				if(gSpcIo.apShareBufWrite>=gSpcIo.apShareBufSize){
					gSpcIo.apShareBufWrite = 0;
				}
			}else {
				kal_mem_cpy( (gSpcIo.pApShareBuf)+offset, data, segment*sizeof(uint8));
				kal_mem_cpy( gSpcIo.pApShareBuf,  data+segment, (dataLen-segment)*sizeof(uint8));
				gSpcIo.apShareBufWrite = dataLen-segment;
			}
			
#ifdef SPC_MSG_ORG_VER
			if(!SPCIO_CCCI_MSG_CMD_IS_EXTEND_M2A(bufp->reserved)){
				ccciMsg = SPCIO_CCCI_MSG_ORG_CONSTRUCT_DATA_WO_SHIFT(bufp->reserved, offset, dataLen);
				SpcIO_MsgQueuePut(SPC_ID_AUDIO_A2M_CCCI, ccciMsg, 0, 0);

				return;
			} else 
#endif // SPC_MSG_ORG_VER
			{
				ccciMsg = SPCIO_CCCI_MSG_CONSTRUCT_DATA_CMD_WO_SHIFT(bufp->reserved, dataLen);
				SpcIO_MsgQueuePut(SPC_ID_AUDIO_A2M_CCCI, ccciMsg, offset, 0);
			}

		}
		
		
#endif //__SDIO_DEVICE_CONNECTIVITY__
	}


}
 
void SpcIO_M2A_hisr(CCCI_BUFF_T *bufp)
{
 
}

// Mail box
kal_bool spcIO_sendMsgViaCCCI(kal_uint32 msg, kal_uint32 revMsg)
{
	kal_uint32 ret, i = 0;
	
   kal_trace(TRACE_INFO, SPCIO_SEND_M2A_MESSAGE, msg, revMsg);

	SpcIO_MsgQueuePut(SPC_ID_AUDIO_M2A_CCCI, msg, revMsg, 1); 
	
	// ret = ccci_mailbox_write(CCCI_PCM_CHANNEL, msg);   
	ret = ccci_mailbox_write_with_reserved(CCCI_PCM_CHANNEL, msg, revMsg);   

   while( ret != CCCI_SUCCESS ){
      kal_trace(TRACE_INFO, SPCIO_SEND_M2A_MESSAGE_FAIL, msg, ret);

      if(i == 3) {
			kal_trace(TRACE_INFO, SPCIO_SEND_M2A_MESSAGE_FAIL_END, msg, ret);
			break;
      }
      i++;
      kal_sleep_task(1);
      // ret = ccci_mailbox_write(CCCI_PCM_CHANNEL, msg);
      ret = ccci_mailbox_write_with_reserved(CCCI_PCM_CHANNEL, msg, revMsg);
   }

   return (kal_bool)(ret == CCCI_SUCCESS);
}

// Stream
/**
	@msg: CCCI message what to put in 
*/
kal_bool spcIO_sendDataViaCCCI(kal_uint32 msg, void *buf, kal_uint32 bufLength)
{

	kal_uint32 ret, i = 0;
	
   kal_trace(TRACE_INFO, SPCIO_SEND_M2A_DATA, msg);

	SpcIO_MsgQueuePut(SPC_ID_AUDIO_M2A_CCCI, msg, bufLength, 1); 

	ret = ccci_stream_write_with_reserved(CCCI_PCM_CHANNEL, (kal_uint32)buf, bufLength, msg);
	
   while( ret != CCCI_SUCCESS ){
      kal_trace(TRACE_INFO, SPCIO_SEND_M2A_DATA_FAIL, msg, ret);

      if(i == 3) {
			kal_trace(TRACE_INFO, SPCIO_SEND_M2A_DATA_FAIL_END, msg, ret);
			break;
      }
      i++;
      kal_sleep_task(1);
      ret = ccci_stream_write_with_reserved(CCCI_PCM_CHANNEL,(kal_uint32)buf, bufLength, msg);
   }

   return (kal_bool)(ret == CCCI_SUCCESS);

}

// ----------------------------------------------------------------------------
// Share Buffer(CCCI over CHIF) / Stream Temp Buffer(CCCI over SDIO) Related
// ----------------------------------------------------------------------------

AUD_CCCI_MSG_T spcIO_msgMapper(SPCIO_MSG_FROM_SPC spcIoMsg)
{
	AUD_CCCI_MSG_T ccciMsgCmd = 0; 
	switch(spcIoMsg){
#ifdef SPC_MSG_ORG_VER	

				
		case SPCIO_MSG_FROM_SPC_SPH_ON_ACK: ccciMsgCmd = MSG_M2A_SPH_CTRL_SPH_ON_ACK; break; 
		case SPCIO_MSG_FROM_SPC_SPH_OFF_ACK: ccciMsgCmd = MSG_M2A_SPH_CTRL_SPH_OFF_ACK; break; 
		case SPCIO_MSG_FROM_SPC_SET_SPH_MODE_ACK: ccciMsgCmd = MSG_M2A_SPH_CTRL_SET_SPH_MODE_ACK; break;
		// case SPCIO_MSG_FROM_SPC_CTRL_SPH_ENH_ACK: ccciMsgCmd = ; break;
		// case SPCIO_MSG_FROM_SPC_CONFIG_SPH_ENH_ACK: ccciMsgCmd = ; break;		
		case SPCIO_MSG_FROM_SPC_SET_ACOUSTIC_LOOPBACK_ACK: ccciMsgCmd = MSG_M2A_SPH_CTRL_SET_ACOUSTIC_LOOPBACK_ACK; break;
		case SPCIO_MSG_FROM_SPC_QUERY_MD_CAPABILITY_ACK: ccciMsgCmd = MSG_M2A_SPH_CTRL_GET_MD_CAPABILITY_ACK; break; 
		case SPCIO_MSG_FROM_SPC_SET_MD_CAPABILITY_ACK: ccciMsgCmd = MSG_M2A_SPH_CTRL_SET_MD_CAPABILITY_ACK; break; 
		// case SPCIO_MSG_FROM_SPC_PRINT_SPH_COEFF_ACK: ccciMsgCmd = ; break;
		// case SPCIO_MSG_FROM_SPC_SPH_ON_FOR_HOLD_CALL_ACK: ccciMsgCmd = : break;
		
		case SPCIO_MSG_FROM_SPC_PNW_ON_ACK: ccciMsgCmd = MSG_M2A_DATA_TRANSFER_PCM_ON_ACK; break;
		case SPCIO_MSG_FROM_SPC_PNW_OFF_ACK: ccciMsgCmd = MSG_M2A_DATA_TRANSFER_PCM_OFF_ACK; break;
		case SPCIO_MSG_FROM_SPC_RECORD_ON_ACK: ccciMsgCmd = MSG_M2A_DATA_TRANSFER_RECORD_ON_ACK; break;
		case SPCIO_MSG_FROM_SPC_RECORD_OFF_ACK: ccciMsgCmd = MSG_M2A_DATA_TRANSFER_RECORD_OFF_ACK; break;
		case SPCIO_MSG_FROM_SPC_DMNR_RECPLAY_ON_ACK: ccciMsgCmd = MSG_M2A_DATA_TRANSFER_DMRN_RECPLAY_ON_ACK; break;
		case SPCIO_MSG_FROM_SPC_DMNR_RECPLAY_OFF_ACK: ccciMsgCmd = MSG_M2A_DATA_TRANSFER_DMRN_RECPLAY_OFF_ACK; break;
		case SPCIO_MSG_FROM_SPC_DMNR_REC_ONLY_ON_ACK: ccciMsgCmd = MSG_M2A_DATA_TRANSFER_DMRN_REC_ONLY_ON_ACK; break;
		case SPCIO_MSG_FROM_SPC_DMNR_REC_ONLY_OFF_ACK: ccciMsgCmd = MSG_M2A_DATA_TRANSFER_DMRN_REC_ONLY_OFF_ACK; break;
		
		case SPCIO_MSG_FROM_SPC_CTM_ON_ACK: ccciMsgCmd = MSG_M2A_AP_CTM_ON_ACK; break;
		case SPCIO_MSG_FROM_SPC_CTM_OFF_ACK: ccciMsgCmd = MSG_M2A_AP_CTM_OFF_ACK; break;
		// case SPCIO_MSG_FROM_SPC_CTM_NEGOTIATION_ACK: ccciMsgCmd = MSG_M2A_AP_START_NEGOTIATION_ACK; break;
		case SPCIO_MSG_FROM_SPC_BGSND_ON_ACK: ccciMsgCmd = MSG_M2A_AP_BGSND_ON_ACK; break;
		case SPCIO_MSG_FROM_SPC_BGSND_OFF_ACK: ccciMsgCmd = MSG_M2A_AP_BGSND_OFF_ACK; break;
		case SPCIO_MSG_FROM_SPC_BGSND_CONFIG_ACK: ccciMsgCmd = MSG_M2A_AP_BGSND_CONFIG_ACK; break;
		
		
		case SPCIO_MSG_FROM_SPC_PNW_DLDATA_REQUEST: ccciMsgCmd = MSG_M2A_DATA_REQUEST_PNW_DLDATA; break;
		case SPCIO_MSG_FROM_SPC_BGS_DATA_REQUEST: ccciMsgCmd = MSG_M2A_DATA_REQUEST_BGSND_DATA; break;
		// case SPCIO_MSG_FROM_SPC_CTM_DATA_REQUEST: ccciMsgCmd = ; break;
		
		case SPCIO_MSG_FROM_SPC_PNW_ULDATA_NOTIFY: ccciMsgCmd = MSG_M2A_DATA_NOTIFY_PNW_ULREAD; break;
		case SPCIO_MSG_FROM_SPC_REC_DATA_NOTIFY: ccciMsgCmd = MSG_M2A_DATA_NOTIFY_REC_DATA; break;
		
		
		case SPCIO_MSG_FROM_SPC_EM_DATA_REQUEST: ccciMsgCmd = MSG_M2A_DATA_REQUEST_EM_DATA; break;
		case SPCIO_MSG_FROM_SPC_EM_INCALL_ACK: ccciMsgCmd = MSG_M2A_MISC_EM_INCALL; break;
		case SPCIO_MSG_FROM_SPC_EM_DMNR_ACK: ccciMsgCmd = MSG_M2A_MISC_EM_DMNR; break;
		case SPCIO_MSG_FROM_SPC_EM_WB_ACK: ccciMsgCmd = MSG_M2A_MISC_EM_WB; break;

		// case SPCIO_MSG_FROM_SPC_VIBSPK_PARAMETER_ACK: ccciMsgCmd = MSG_M2A_VIBSPK_PARAMETER_ACK; break; // new added
#else 
	

		case SPCIO_MSG_FROM_SPC_SPH_ON_ACK: ccciMsgCmd = MSG_M2A_SPH_ON_ACK; break; 
		case SPCIO_MSG_FROM_SPC_SPH_OFF_ACK: ccciMsgCmd = MSG_M2A_SPH_OFF_ACK; break; 
		case SPCIO_MSG_FROM_SPC_SET_SPH_MODE_ACK: ccciMsgCmd = MSG_M2A_SET_SPH_MODE_ACK; break;
		case SPCIO_MSG_FROM_SPC_CTRL_SPH_ENH_ACK: ccciMsgCmd = MSG_M2A_CTRL_SPH_ENH_ACK; break;
		case SPCIO_MSG_FROM_SPC_CONFIG_SPH_ENH_ACK: ccciMsgCmd = MSG_M2A_CONFIG_SPH_ENH_ACK; break;		
		case SPCIO_MSG_FROM_SPC_SET_ACOUSTIC_LOOPBACK_ACK: ccciMsgCmd = MSG_M2A_SET_ACOUSTIC_LOOPBACK_ACK; break;
		/*
		// phase out from MT6589
		case SPCIO_MSG_FROM_SPC_QUERY_MD_CAPABILITY_ACK: ccciMsgCmd = MSG_M2A_QUERY_MD_CAPABILITY_ACK; break; 
		case SPCIO_MSG_FROM_SPC_SET_MD_CAPABILITY_ACK: ccciMsgCmd = MSG_M2A_SET_MD_CAPABILITY_ACK; break; 
		*/
		case SPCIO_MSG_FROM_SPC_PRINT_SPH_COEFF_ACK: ccciMsgCmd = MSG_M2A_PRINT_SPH_COEFF_ACK; break;
		case SPCIO_MSG_FROM_SPC_SPH_ON_FOR_HOLD_CALL_ACK: ccciMsgCmd = MSG_M2A_SPH_ON_FOR_HOLD_CALL_ACK; break;

		case SPCIO_MSG_FROM_SPC_PNW_ON_ACK: ccciMsgCmd = MSG_M2A_PNW_ON_ACK; break;
		case SPCIO_MSG_FROM_SPC_PNW_OFF_ACK: ccciMsgCmd = MSG_M2A_PNW_OFF_ACK; break;
		case SPCIO_MSG_FROM_SPC_RECORD_ON_ACK: ccciMsgCmd = MSG_M2A_RECORD_ON_ACK; break;
		case SPCIO_MSG_FROM_SPC_RECORD_OFF_ACK: ccciMsgCmd = MSG_M2A_RECORD_OFF_ACK; break;
		case SPCIO_MSG_FROM_SPC_DMNR_RECPLAY_ON_ACK: ccciMsgCmd = MSG_M2A_DMNR_RECPLAY_ON_ACK; break;
		case SPCIO_MSG_FROM_SPC_DMNR_RECPLAY_OFF_ACK: ccciMsgCmd = MSG_M2A_DMNR_RECPLAY_OFF_ACK; break;
		case SPCIO_MSG_FROM_SPC_DMNR_REC_ONLY_ON_ACK: ccciMsgCmd = MSG_M2A_DMNR_REC_ONLY_ON_ACK; break;
		case SPCIO_MSG_FROM_SPC_DMNR_REC_ONLY_OFF_ACK: ccciMsgCmd = MSG_M2A_DMNR_REC_ONLY_OFF_ACK; break;
		case SPCIO_MSG_FROM_SPC_PCM_REC_ON_ACK: ccciMsgCmd = MSG_M2A_PCM_REC_ON_ACK; break;
		case SPCIO_MSG_FROM_SPC_PCM_REC_OFF_ACK: ccciMsgCmd = MSG_M2A_PCM_REC_OFF_ACK; break;
		case SPCIO_MSG_FROM_SPC_VM_REC_ON_ACK: ccciMsgCmd = MSG_M2A_VM_REC_ON_ACK; break;
		case SPCIO_MSG_FROM_SPC_VM_REC_OFF_ACK: ccciMsgCmd = MSG_M2A_VM_REC_OFF_ACK; break;

		case SPCIO_MSG_FROM_SPC_CTM_ON_ACK: ccciMsgCmd = MSG_M2A_CTM_ON_ACK; break;
		case SPCIO_MSG_FROM_SPC_CTM_OFF_ACK: ccciMsgCmd = MSG_M2A_CTM_OFF_ACK; break;
		case SPCIO_MSG_FROM_SPC_CTM_DUMP_DEBUG_FILE_ACK: ccciMsgCmd = MSG_M2A_CTM_DUMP_DEBUG_FILE_ACK; break;
		case SPCIO_MSG_FROM_SPC_BGSND_ON_ACK: ccciMsgCmd = MSG_M2A_BGSND_ON_ACK; break;
		case SPCIO_MSG_FROM_SPC_BGSND_OFF_ACK: ccciMsgCmd = MSG_M2A_BGSND_OFF_ACK; break;
		case SPCIO_MSG_FROM_SPC_BGSND_CONFIG_ACK: ccciMsgCmd = MSG_M2A_BGSND_CONFIG_ACK; break;


		case SPCIO_MSG_FROM_SPC_PNW_DLDATA_REQUEST: ccciMsgCmd = MSG_M2A_PNW_DL_DATA_REQUEST; break;
		case SPCIO_MSG_FROM_SPC_BGS_DATA_REQUEST: ccciMsgCmd = MSG_M2A_BGSND_DATA_REQUEST; break;
		case SPCIO_MSG_FROM_SPC_CTM_DATA_REQUEST: ccciMsgCmd = MSG_M2A_CTM_DATA_REQUEST; break;

		case SPCIO_MSG_FROM_SPC_PNW_ULDATA_NOTIFY: ccciMsgCmd = MSG_M2A_PNW_UL_DATA_NOTIFY; break;
		case SPCIO_MSG_FROM_SPC_REC_DATA_NOTIFY: ccciMsgCmd = MSG_M2A_REC_DATA_NOTIFY; break;
		case SPCIO_MSG_FROM_SPC_CTM_DEBUG_DATA_NOTIFY: ccciMsgCmd = MSG_M2A_CTM_DEBUG_DATA_NOTIFY; break;
		case SPCIO_MSG_FROM_SPC_PCM_REC_DATA_NOTIFY: ccciMsgCmd = MSG_M2A_PCM_REC_DATA_NOTIFY; break;
		case SPCIO_MSG_FROM_SPC_VM_REC_DATA_NOTIFY: ccciMsgCmd = MSG_M2A_VM_REC_DATA_NOTIFY; break;

		case SPCIO_MSG_FROM_SPC_EM_DATA_REQUEST: ccciMsgCmd = MSG_M2A_EM_DATA_REQUEST; break;
		case SPCIO_MSG_FROM_SPC_EM_INCALL_ACK: ccciMsgCmd = MSG_M2A_EM_INCALL_ACK; break;
		case SPCIO_MSG_FROM_SPC_EM_DMNR_ACK: ccciMsgCmd = MSG_M2A_EM_DMNR_ACK; break;
		case SPCIO_MSG_FROM_SPC_EM_WB_ACK: ccciMsgCmd = MSG_M2A_EM_WB_ACK; break;

		case SPCIO_MSG_FROM_SPC_VIBSPK_PARAMETER_ACK: ccciMsgCmd = MSG_M2A_VIBSPK_PARAMETER_ACK; break;
		case SPCIO_MSG_FROM_SPC_NXP_SMARTPA_PARAMETER_ACK: ccciMsgCmd = MSG_M2A_NXP_SMARTPA_PARAMETER_ACK; break;
		
#endif // #ifdef SPC_MSG_ORG_VER	
	default: 
			ASSERT(0);	

	}
	return ccciMsgCmd;
}
kal_uint16 SpcIo_GetDataFromAp(const kal_uint16 offset, const kal_uint16 length, void *buf)
{
	kal_uint16 procLen;
	kal_uint16 curOffset;
	
	curOffset = offset;
	procLen = length;
	if((curOffset + procLen) > gSpcIo.apShareBufSize) {
		kal_uint32 segment = gSpcIo.apShareBufSize - curOffset;
		kal_uint8 *pCurrent = buf;
		kal_mem_cpy(pCurrent, gSpcIo.pApShareBuf+curOffset, sizeof(uint8)*segment);
		pCurrent += segment;
		
		segment = procLen - segment;
		kal_mem_cpy(pCurrent, gSpcIo.pApShareBuf, sizeof(uint8)*segment);

		// move offset pointer
		curOffset = segment;
	} else {
		kal_mem_cpy(buf, gSpcIo.pApShareBuf+curOffset, sizeof(uint8)*procLen);

		// move offset pointer
		curOffset = offset + procLen ;
	}

	// move read pointer
	//gSpcIo.apShareBufRead = curOffset;

	// provide current position
	return curOffset;
		
}

/**
	@offset: [Input] 
	@length: [Input] unit int byte
	@headerLen: [Input] unit int byte

	@header: [Output]
	@buf: [Output]
	
*/
void SpcIO_GetDataFromAp_inOneTime(kal_uint16 offset, kal_uint16 length, kal_int16 headerLen,
	void *header, void *buf)
{
	kal_int16 procLen;
	kal_uint16 curOffset;

	// length checking: total processing length should be longer then header
	kal_prompt_trace(MOD_L1SP, "length: %d, headerLen: %d", length, headerLen);	
	ASSERT(length >= headerLen);  
	
	//header
	curOffset = SpcIo_GetDataFromAp(offset, headerLen, header);

	kal_prompt_trace(MOD_L1SP, "header[0]=%x, header[1]=%x, header[2]=%x", *((kal_uint16 *)header), *((kal_uint16 *)header+1), *((kal_uint16 *)header+2));
		
	//data
	procLen = length - headerLen;
	if(procLen >0) { // prevent empty data
		SpcIo_GetDataFromAp(curOffset, procLen, buf);
	} else {
		buf = NULL;
	}
}

void spcIO_copyDataToMdShareBuf(const kal_uint8 *pSrc, const kal_int16 srcLen)
{
	kal_uint8 *pSrcCur; 
	kal_uint16 curOffset = gSpcIo.mdShareBufWrite;
	
	pSrcCur = (kal_uint8 *)pSrc;
	
	if((curOffset + srcLen) > gSpcIo.mdShareBufSize) {
		// cut to segment 
		kal_int16 segment = gSpcIo.mdShareBufSize -  curOffset;

		kal_mem_cpy( (kal_uint8 *)(gSpcIo.pMdShareBuf + curOffset), pSrcCur, sizeof(uint8)*segment);
		pSrcCur = pSrcCur + segment; 

		segment = srcLen - segment;
		kal_mem_cpy(gSpcIo.pMdShareBuf, pSrcCur, sizeof(uint8)*segment);

		// move offset pointer
		curOffset = segment;
	} else {
		kal_mem_cpy((kal_uint8 *)(gSpcIo.pMdShareBuf+curOffset), pSrcCur, sizeof(uint8)*srcLen);

		// move offset pointer
		curOffset = curOffset + srcLen ;
	}

	gSpcIo.mdShareBufWrite = curOffset;

	}
/**
	if the result is fail, log will leave inside
	@headerBuf: 
	@headerLen:
	@srcBuf:
	@srcLen:
	@msg:

	@return: successful send or not! 
*/
bool SpcIO_WriteDataToAp(void *headerBuf, kal_int16 headerLen, 
	void *srcBuf1, kal_int16 srcLen1, void *srcBuf2, kal_int16 srcLen2, 
	SPCIO_MSG_FROM_SPC spcIoMsg)
{
	kal_uint16 length;
	
	kal_bool sendResult = KAL_FALSE;
	AUD_CCCI_MSG_T msg;



#ifdef __SDIO_DEVICE_CONNECTIVITY__	
	kal_uint8 *sendData;
	kal_uint16 ccciStreamSize; 

	sendData = gSpcIo.pMdShareBuf;
	length = headerLen + srcLen1 + srcLen2;	
	ccciStreamSize = length + SPCIO_CCCI_STREAM_BUF_RESERVED_SIZE;
	
	kal_mem_cpy((kal_uint8 *)(sendData+ SPCIO_CCCI_STREAM_BUF_RESERVED_SIZE) , headerBuf, headerLen);
	kal_mem_cpy((kal_uint8 *)(sendData+ headerLen+SPCIO_CCCI_STREAM_BUF_RESERVED_SIZE), srcBuf1, srcLen1);
	if(0 != srcLen2) {
		kal_mem_cpy((kal_uint8 *)(sendData+ srcLen1+ headerLen+SPCIO_CCCI_STREAM_BUF_RESERVED_SIZE), srcBuf2, srcLen2);
	}
	
	
	// mapper 
	msg = spcIO_msgMapper(spcIoMsg);
		
	//send via CCCI stream
#ifdef SPC_MSG_ORG_VER	
	if(!SPCIO_CCCI_MSG_CMD_IS_EXTEND_M2A(msg)){
		sendResult = spcIO_sendDataViaCCCI(SPCIO_CCCI_MSG_ORG_CONSTRUCT_DATA(msg, 0, length),
			sendData, ccciStreamSize);
		
	} else 
#endif // SPC_MSG_ORG_VER	
	{	
		sendResult = spcIO_sendDataViaCCCI(SPCIO_CCCI_MSG_CONSTRCUT_DATA_CMD(msg, length), 
			sendData, ccciStreamSize);
	}

#else //via CHIF
	kal_uint16 offset;
	//keep starting address before put data into share buffer
	length = headerLen + srcLen1 + srcLen2;
	offset = gSpcIo.mdShareBufWrite;
	
	// put to share buffer
	if((0 != headerLen) && (NULL != headerBuf)) {
		spcIO_copyDataToMdShareBuf(headerBuf, headerLen);
	}
	spcIO_copyDataToMdShareBuf(srcBuf1, srcLen1);	
	if(0 != srcLen2) {
		spcIO_copyDataToMdShareBuf(srcBuf2, srcLen2);	
	}
	
	// mapper 
	msg = spcIO_msgMapper(spcIoMsg);
		
	// send data notification message
#ifdef SPC_MSG_ORG_VER		
	if(!SPCIO_CCCI_MSG_CMD_IS_EXTEND_M2A(msg)){
		sendResult = spcIO_sendMsgViaCCCI(SPCIO_CCCI_MSG_ORG_CONSTRUCT_DATA(msg, offset, length), 0);
	} else 
#endif //SPC_MSG_ORG_VER
	{
		sendResult = spcIO_sendMsgViaCCCI(SPCIO_CCCI_MSG_CONSTRCUT_DATA_CMD(msg, length), offset);
	}

#endif // __SDIO_DEVICE_CONNECTIVITY__

	return sendResult;

}



kal_bool SpcIO_SendMsgToAp(SPCIO_MSG_FROM_SPC spcIoMsg, uint32 data1, uint32 reserve)
{
	uint32 msgContent;
	AUD_CCCI_MSG_T ccciMsgCmd; 

	// mapper 
	ccciMsgCmd = spcIO_msgMapper(spcIoMsg);
	
#ifdef SPC_MSG_ORG_VER	
	msgContent = SPCIO_CCCI_MSG_ORG_CONSTRCUT_CMD(ccciMsgCmd, data1);
	return spcIO_sendMsgViaCCCI(msgContent, 0);
#else 
	msgContent = SPCIO_CCCI_MSG_CONSTRCUT_CMD(ccciMsgCmd, data1);
	return spcIO_sendMsgViaCCCI(msgContent, reserve);
#endif // SPC_MSG_ORG_VER

}

// ----------------------------------------------------------------------------
// AUDL running functions Related
// ----------------------------------------------------------------------------

void SpcIO_Msg_Handler_inAudL(ilm_struct *ilm_ptr)
{

	//process internal queue
	// if(ilmMsg->msg_id == MSG_ID_AUDIO_A2M_CCCI) 
	if(MSG_ID_AUDIO_A2M_CCCI == ilm_ptr->msg_id) 
	{
		while (!RB_EMPTY(gSpcIo.ccciq)) 
   	{
   		SPCIO_MESSAGE_T msg; 
		
	   	spcIO_MsgQueueGet(&msg);
			switch (msg.message_id)
	      {
				case SPC_ID_AUDIO_A2M_CCCI:
					if(SPCIO_QUEUE_STATUS_READY == gSpcIo.ccciqStatue) {
#ifdef SPC_MSG_ORG_VER
						spc_AP_Command_Handler(msg.message_data, msg.message_dataRev);
#else
						spc_A2M_MsgHandler(msg.message_data, msg.message_dataRev);
#endif // SPC_MSG_ORG_VER
					} else {
						// leave a log
						kal_trace(TRACE_INFO, SPCIO_INVALID_MSG_BEFORE_QUEUE_READY, msg.message_id, msg.message_data, msg.message_dataRev);
					}
					break;				
			
				case SPC_ID_AUDIO_M2A_CCCI:
					//directly send when call spcIO_sendMsgViaCCCI() or spcIO_sendDataViaCCCI()
					break;
					
				case SPC_ID_AUDIO_STRM_PNW_UL_DATA_NOTIFY: 
					//handle the data to pcmNway data to AP					
					spc_pcmNWay_sendUlData();					
					break;

				case SPC_ID_AUDIO_STRM_PNW_DL_DATA_REQUEST:
					spc_pcmNWay_requestDlData();
					break;
		
				case SPC_ID_AUDIO_STRM_BGSND_DATA_REQUEST:
					spc_bgSnd_requestData();
         		break;
			
			   case SPC_ID_SPEECH_CUSTOM_DATA_REQUEST:
					// Boot UP to request EM data					
         		SpcIO_SendMsgToAp(SPCIO_MSG_FROM_SPC_EM_DATA_REQUEST, 0, 0);
					gSpcIo.ccciqStatue = SPCIO_QUEUE_STATUS_READY;
					break;

				case SPC_ID_AUDIO_STRM_PCMREC_DATA_NOTIFY: //pcm record
					spc_record_sendPcmData();
					break;

				case SPC_ID_AUDIO_STRM_VMREC_DATA_NOTIFY: // vm record
					spc_record_sendVmData();
					break;

				case SPC_ID_AUDIO_STRM_CTM_DUMP_DATA_NOTIFY:
					spc_ctm_sendDumpDebugData();
					break;

				case SPC_ID_AUDIO_CONTROL_BGSND_CLOSE: // close background sound and send back the message
					spc_BgSndClose();
					break;
					
				default:
	         	kal_trace(TRACE_INFO, SPCIO_INVALID_MSG, msg.message_id, msg.message_data, msg.message_dataRev);
	      }
			
			_spcIO_MsgLog(&msg, 2); 
		}
	} else {
		ASSERT(0); // only spcIO HISR will send message to AUDL in orrect case
	}
	
	
}

// ============================================================================

void SpcIO_Init(void)
{
	// do initail when AUDL first running. 
	// gSpcIo.pIlm = allocate_ilm(MOD_L1SP);
	
#ifdef __SDIO_DEVICE_CONNECTIVITY__
	gSpcIo.pApShareBuf = gSpcIo_ApShareBuffer;
	gSpcIo.apShareBufSize = SPCIO_SHARE_BUF_SIZE;

	gSpcIo.pMdShareBuf = gSpcIo_MdShareBuffer;
	gSpcIo.mdShareBufSize = SPCIO_SHARE_BUF_SIZE;
#else	
	gSpcIo.pApShareBuf = (uint8 *)ccci_query_sharemem_addr(CCCI_PCM_CHANNEL, &gSpcIo.apShareBufSize);
	gSpcIo.apShareBufSize >>= 1;
	
	gSpcIo.pMdShareBuf = (uint8 *)gSpcIo.pApShareBuf + gSpcIo.apShareBufSize;
	gSpcIo.mdShareBufSize = gSpcIo.apShareBufSize;
#endif // __SDIO_DEVICE_CONNECTIVITY__

	gSpcIo.mdShareBufWrite = 0;
	gSpcIo.apShareBufWrite = 0;

	gSpcIo.ccciqStatue = SPCIO_QUEUE_STATUS_INIT;
	
	Spc_Init();
	
#ifdef SPH_CHIP_BACK_PHONECALL_USED
	// do not send notify to ap		
	gSpcIo.ccciqStatue = SPCIO_QUEUE_STATUS_READY;
#else 

	//final step to notify AP 
	SpcIO_MsgQueuePut(SPC_ID_SPEECH_CUSTOM_DATA_REQUEST, 0, 0, 0);
	msg_send4(MOD_L1SP, MOD_MED, AUDIO_SAP, MSG_ID_AUDIO_A2M_CCCI);
#endif // SPH_CHIP_BACK_PHONECALL_USE

	// final step
	gSpcIo.isTaskInitialize = KAL_TRUE;
}

