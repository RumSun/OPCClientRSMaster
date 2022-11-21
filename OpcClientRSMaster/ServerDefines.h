#pragma once
#if !defined(SERVERDEFINES_H)
#define SERVERDEFINES_H

/******************************************************************************
**			OPC Server & Updaterate
******************************************************************************/
#define OPCCLIENT_SERVER_PROGID "Lectus.OPC.1"
#define OPCCLIENT_GROUP_NAME "Group_RSClient"
#define OPCCLIENT_GROUP_UPDATERATE 500

/******************************************************************************
**			Status
******************************************************************************/
//#define OPCCLIENT_MASTER_SLAVE_ITEM_COUNT 4
//#define OPCCLIENT_MASTER_STATE "DP:[CP 5613 A2]Masterstate"
//#define OPCCLIENT_SLAVE_TYPE "DP:[CP 5613 A2]Slave005MiscSlvType"
//#define OPCCLIENT_SLAVE_STATE "DP:[CP 5613 A2]Slave005SlvState"
//#define OPCCLIENT_SLAVE_DIAGNOSIS "DP:[CP 5613 A2]Slave005SlvDiag"

/******************************************************************************
**			6 x Analog Input / 1 x Analog Output
******************************************************************************/
#define OPCCLIENT_GROUP_ANALOG_REQUESTEDDATATYPE VT_BSTR

#define OPCCLIENT_ANALOG_INPUT_ITEM_COUNT 42
#define OPCCLIENT_ANALOG_INPUT_ITEM1 "Rooftop_2.Temperature.SATemp"
#define OPCCLIENT_ANALOG_INPUT_ITEM2 "Rooftop_2.Temperature.RATemp"
#define OPCCLIENT_ANALOG_INPUT_ITEM3 "Rooftop_2.Temperature.RWTemp"
#define OPCCLIENT_ANALOG_INPUT_ITEM4 "Rooftop_2.Temperature.OATemp"
#define OPCCLIENT_ANALOG_INPUT_ITEM5 "Rooftop_2.AOuts.Y_FAD"
#define OPCCLIENT_ANALOG_INPUT_ITEM6 "Rooftop_2.AOuts.Y_Heating"

#define OPCCLIENT_ANALOG_INPUT_ITEM31 "Rooftop_3.Temperature.SATemp"
#define OPCCLIENT_ANALOG_INPUT_ITEM32 "Rooftop_3.Temperature.RATemp"
#define OPCCLIENT_ANALOG_INPUT_ITEM33 "Rooftop_3.Temperature.RWTemp"
#define OPCCLIENT_ANALOG_INPUT_ITEM34 "Rooftop_3.Temperature.OATemp"
#define OPCCLIENT_ANALOG_INPUT_ITEM35 "Rooftop_3.AOuts.Y_FAD"
#define OPCCLIENT_ANALOG_INPUT_ITEM36 "Rooftop_3.AOuts.Y_Heating"

#define OPCCLIENT_ANALOG_INPUT_ITEM41 "Rooftop_4.Temperature.SATemp"
#define OPCCLIENT_ANALOG_INPUT_ITEM42 "Rooftop_4.Temperature.RATemp"
#define OPCCLIENT_ANALOG_INPUT_ITEM43 "Rooftop_4.Temperature.RWTemp"
#define OPCCLIENT_ANALOG_INPUT_ITEM44 "Rooftop_4.Temperature.OATemp"
#define OPCCLIENT_ANALOG_INPUT_ITEM45 "Rooftop_4.AOuts.Y_FAD"
#define OPCCLIENT_ANALOG_INPUT_ITEM46 "Rooftop_4.AOuts.Y_Heating"

#define OPCCLIENT_ANALOG_INPUT_ITEM51 "Rooftop_5.Temperature.SATemp"
#define OPCCLIENT_ANALOG_INPUT_ITEM52 "Rooftop_5.Temperature.RATemp"
#define OPCCLIENT_ANALOG_INPUT_ITEM53 "Rooftop_5.Temperature.RWTemp"
#define OPCCLIENT_ANALOG_INPUT_ITEM54 "Rooftop_5.Temperature.OATemp"
#define OPCCLIENT_ANALOG_INPUT_ITEM55 "Rooftop_5.AOuts.Y_FAD"
#define OPCCLIENT_ANALOG_INPUT_ITEM56 "Rooftop_5.AOuts.Y_Heating"

#define OPCCLIENT_ANALOG_INPUT_ITEM61 "Rooftop_6.Temperature.SATemp"
#define OPCCLIENT_ANALOG_INPUT_ITEM62 "Rooftop_6.Temperature.RATemp"
#define OPCCLIENT_ANALOG_INPUT_ITEM63 "Rooftop_6.Temperature.RWTemp"
#define OPCCLIENT_ANALOG_INPUT_ITEM64 "Rooftop_6.Temperature.OATemp"
#define OPCCLIENT_ANALOG_INPUT_ITEM65 "Rooftop_6.AOuts.Y_FAD"
#define OPCCLIENT_ANALOG_INPUT_ITEM66 "Rooftop_6.AOuts.Y_Heating"

#define OPCCLIENT_ANALOG_INPUT_ITEM71 "Rooftop_7.Temperature.SATemp"
#define OPCCLIENT_ANALOG_INPUT_ITEM72 "Rooftop_7.Temperature.RATemp"
#define OPCCLIENT_ANALOG_INPUT_ITEM73 "Rooftop_7.Temperature.RWTemp"
#define OPCCLIENT_ANALOG_INPUT_ITEM74 "Rooftop_7.Temperature.OATemp"
#define OPCCLIENT_ANALOG_INPUT_ITEM75 "Rooftop_7.AOuts.Y_FAD"
#define OPCCLIENT_ANALOG_INPUT_ITEM76 "Rooftop_7.AOuts.Y_Heating"

#define OPCCLIENT_ANALOG_INPUT_ITEM81 "Rooftop_8.Temperature.SATemp"
#define OPCCLIENT_ANALOG_INPUT_ITEM82 "Rooftop_8.Temperature.RATemp"
#define OPCCLIENT_ANALOG_INPUT_ITEM83 "Rooftop_8.Temperature.RWTemp"
#define OPCCLIENT_ANALOG_INPUT_ITEM84 "Rooftop_8.Temperature.OATemp"
#define OPCCLIENT_ANALOG_INPUT_ITEM85 "Rooftop_8.AOuts.Y_FAD"
#define OPCCLIENT_ANALOG_INPUT_ITEM86 "Rooftop_8.AOuts.Y_Heating"

#define OPCCLIENT_ANALOG_OUTPUT_ITEM_COUNT 7
#define OPCCLIENT_ANALOG_OUTPUT_ITEM1 "Rooftop_2.Setpoints.SATemp_SP"
#define OPCCLIENT_ANALOG_OUTPUT_ITEM31 "Rooftop_3.Setpoints.SATemp_SP"
#define OPCCLIENT_ANALOG_OUTPUT_ITEM41 "Rooftop_4.Setpoints.SATemp_SP"
#define OPCCLIENT_ANALOG_OUTPUT_ITEM51 "Rooftop_5.Setpoints.SATemp_SP"
#define OPCCLIENT_ANALOG_OUTPUT_ITEM61 "Rooftop_6.Setpoints.SATemp_SP"
#define OPCCLIENT_ANALOG_OUTPUT_ITEM71 "Rooftop_7.Setpoints.SATemp_SP"
#define OPCCLIENT_ANALOG_OUTPUT_ITEM81 "Rooftop_8.Setpoints.SATemp_SP"

/******************************************************************************
**			8 Bit Input / 8 Bit Output
******************************************************************************/
#define OPCCLIENT_BIT_REQUESTEDDATATYPE VT_BOOL

#define OPCCLIENT_BITINPUT_ITEM_COUNT 7
#define OPCCLIENT_BITINPUT_ITEM21 "Rooftop_2.Indication.SAF_out"
#define OPCCLIENT_BITINPUT_ITEM31 "Rooftop_3.Indication.SAF_out"
#define OPCCLIENT_BITINPUT_ITEM41 "Rooftop_4.Indication.SAF_out"
#define OPCCLIENT_BITINPUT_ITEM51 "Rooftop_5.Indication.SAF_out"
#define OPCCLIENT_BITINPUT_ITEM61 "Rooftop_6.Indication.SAF_out"
#define OPCCLIENT_BITINPUT_ITEM71 "Rooftop_7.Indication.SAF_out"
#define OPCCLIENT_BITINPUT_ITEM81 "Rooftop_8.Indication.SAF_out"

#define OPCCLIENT_BITOUTPUT_ITEM_COUNT 0
///// добавить включения всех агрегатов!!!!!!!!!!!!
#define OPCCLIENT_BITOUTPUT_ITEM1 "Rooftop_2.OnOff.Com"
#define OPCCLIENT_BITOUTPUT_ITEM31 "Rooftop_3.OnOff.Com"
#define OPCCLIENT_BITOUTPUT_ITEM41 "Rooftop_4.OnOff.Com"
#define OPCCLIENT_BITOUTPUT_ITEM51 "Rooftop_5.OnOff.Com"
#define OPCCLIENT_BITOUTPUT_ITEM61 "Rooftop_6.OnOff.Com"
#define OPCCLIENT_BITOUTPUT_ITEM71 "Rooftop_7.OnOff.Com"
#define OPCCLIENT_BITOUTPUT_ITEM81 "Rooftop_8.OnOff.Com"


/******************************************************************************
**			Control Function
******************************************************************************/
/*#define OPCCLIENT_CONTROLFUNCTION_SERVER_PROGID "Lectus.OPC.1"
#define OPCCLIENT_CONTROLFUNCTION_GROUP_NAME "testgroup2"
#define OPCCLIENT_THREAD_CYCLETIME	50

#define OPCCLIENT_CONTROLFUNCTION_FLOAT_IN_ITEM_COUNT 0
#define OPCCLIENT_CONTROLFUNCTION_FLOAT_IN_ITEM1 ""
#define OPCCLIENT_CONTROLFUNCTION_FLOAT_OUT_ITEM_COUNT 0
#define OPCCLIENT_CONTROLFUNCTION_FLOAT_OUT_ITEM1 ""

#define OPCCLIENT_CONTROLFUNCTION_WORD_IN_ITEM_COUNT 2
#define OPCCLIENT_CONTROLFUNCTION_WORD_IN1 "DP:[CP 5613 A2]Slave005M00_IW0"
#define OPCCLIENT_CONTROLFUNCTION_WORD_IN2 "DP:[CP 5613 A2]Slave005M00_IW2"

#define OPCCLIENT_CONTROLFUNCTION_WORD_OUT_ITEM_COUNT 4
#define OPCCLIENT_CONTROLFUNCTION_WORD_OUT1 "DP:[CP 5613 A2]Slave005M01_QW0"
#define OPCCLIENT_CONTROLFUNCTION_WORD_OUT2 "DP:[CP 5613 A2]Slave005M01_QW2"
#define OPCCLIENT_CONTROLFUNCTION_WORD_OUT3 "DP:[CP 5613 A2]Slave005M01_QW4"
#define OPCCLIENT_CONTROLFUNCTION_WORD_OUT4 "DP:[CP 5613 A2]Slave005M01_QW6"

#define OPCCLIENT_CONTROLFUNCTION_BYTE_IN_ITEM_COUNT 1
#define OPCCLIENT_CONTROLFUNCTION_BYTE_IN1 "DP:[CP 5613 A2]Slave005M02_IB0"
#define OPCCLIENT_CONTROLFUNCTION_BYTE_OUT_ITEM_COUNT 1
#define OPCCLIENT_CONTROLFUNCTION_BYTE_OUT1 "DP:[CP 5613 A2]Slave005M02_QB0"
*/
#endif