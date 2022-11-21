/******************************************************************************

  Declaration CMemoryMap

  File:					MemoryMap.h

  Project:				OpcClientDPMaster

  Class:				Description:
  -----------------------------------------------------------------------------
  CMemoryMap			Class for managment of the process image.

******************************************************************************/
#pragma once

#include "stdafx.h"

class CMemoryMap
{
public:
	/** 
	* construction / destruction
	*/
	CMemoryMap(void);
	~CMemoryMap(void);
	/** 
	* Initializes member-arrays.
	*/
	void Initialize(void);
	/** 
	* Writes given values to member-arrays.
	*/
	void WriteInput(const CVARIANTArray &aValues);
	/** 
	* Reads values from member-arrays.
	*/
	void ReadOutput(CVARIANTArray &aValues);
	/** 
	* Gets a FLOAT.
	*/
	FLOAT GetInFloat(int floatIndex);
	/** 
	* Sets a FLOAT.
	*/
	void SetOutFloat(int floatIndex, FLOAT Value);
	/** 
	* Gets a WORD.
	*/
	WORD GetInWord(int wordIndex);
	/** 
	* Sets a WORD.
	*/
	void SetOutWord(int wordIndex, WORD Value);
	/** 
	* Gets a byte.
	*/
	BOOL GetInByte(int byteIndex);
	/** 
	* Sets a byte.
	*/
	void SetOutByte(int byteIndex, BYTE value);
	/** 
	* Gets a byte.
	*/
	BOOL GetInBool(int byteIndex, int bitIndex);
	/** 
	* Sets a bit.
	*/
	void SetOutBool(int byteIndex, int bitIndex, BOOL value);

private:
	/** Real-array *********************************/
	CFLOATArray m_aFloats;
	//CDOUBLEArray m_aDWords;
	/** Word-array **********************************/
	CWordArray	m_aWords;
	/** Byte-array **********************************/
	CByteArray	m_aBytes;
};
