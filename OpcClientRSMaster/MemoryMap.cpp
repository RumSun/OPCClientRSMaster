/******************************************************************************

  Implementation CMemoryMap

  File:					MemoryMap.cpp

  Project:				OpcClientDPMaster

  Class:				Description:
  -----------------------------------------------------------------------------
  CMemoryMap			Implementation CMemoryMap

******************************************************************************/
#include "StdAfx.h"
#include ".\memorymap.h"

/* ----------------------------------------------------------------------------
    Begin Class		CMemoryMap

	constructors / destructors
-----------------------------------------------------------------------------*/
CMemoryMap::CMemoryMap(void)
{
	m_aFloats.RemoveAll();
}
CMemoryMap::~CMemoryMap(void)
{
	m_aBytes.RemoveAll();
	m_aWords.RemoveAll();
	m_aFloats.RemoveAll();
}
/* ----------------------------------------------------------------------------
    Class		CMemoryMap
	Method		Initialize
	Description	Initializes the member-arrays with default values (0). 
				Length is defined in ServerDefines.h
-----------------------------------------------------------------------------*/
void CMemoryMap::Initialize(void)
{
	int i;
	int ByteCount = OPCCLIENT_CONTROLFUNCTION_BYTE_IN_ITEM_COUNT + 
					OPCCLIENT_CONTROLFUNCTION_BYTE_OUT_ITEM_COUNT;
	int WordCount = OPCCLIENT_CONTROLFUNCTION_WORD_IN_ITEM_COUNT + 
					OPCCLIENT_CONTROLFUNCTION_WORD_OUT_ITEM_COUNT;
	int FloatCount= OPCCLIENT_CONTROLFUNCTION_FLOAT_IN_ITEM_COUNT +
					OPCCLIENT_CONTROLFUNCTION_FLOAT_OUT_ITEM_COUNT;

	m_aBytes.RemoveAll();
	m_aWords.RemoveAll();
	m_aFloats.RemoveAll();

	for (i=0; i<ByteCount; i++)
	{
		m_aBytes.SetAtGrow(i,0);
	}
	for (i=0; i<WordCount; i++)
	{
		m_aWords.SetAtGrow(i,0);
	}
	for (i=0; i<FloatCount; i++)
	{
		FLOAT f = 0;
		m_aFloats.SetAtGrow(i,f);
	}
}
/* ----------------------------------------------------------------------------
    Class		CMemoryMap
	Method		WriteInput
	Description	Writes the input-items to the member-arrays. The order depends
				on the CControl-member m_aServerHandlesRead, created in 
				CControl::ThreadEnter().
-----------------------------------------------------------------------------*/
void CMemoryMap::WriteInput(const CVARIANTArray &aValues)
{
	int i, offset;
	//write FLOATs
	for (i=0; i<OPCCLIENT_CONTROLFUNCTION_FLOAT_IN_ITEM_COUNT; i++)
	{
		m_aFloats[i] = aValues[i].fltVal;
	}
	//write WORDs
	offset = OPCCLIENT_CONTROLFUNCTION_FLOAT_IN_ITEM_COUNT;
	for (i=0; i<OPCCLIENT_CONTROLFUNCTION_WORD_IN_ITEM_COUNT; i++)
	{
		m_aWords[i] = aValues[i + offset].iVal;
	}
	//write BYTEs
	offset = OPCCLIENT_CONTROLFUNCTION_FLOAT_IN_ITEM_COUNT + OPCCLIENT_CONTROLFUNCTION_WORD_IN_ITEM_COUNT;
	for (i=0; i<OPCCLIENT_CONTROLFUNCTION_BYTE_IN_ITEM_COUNT; i++)
	{
		m_aBytes[i] = aValues[i + offset].bVal;
	}
}
/* ----------------------------------------------------------------------------
    Class		CMemoryMap
	Method		ReadOutput
	Description	Reads output-items from dword-,word- and byte-memberarray and 
				sets them to the given variant-array.
				The order of the variant-array depends on the CControl-member 
				m_aServerHandlesWrite, created in CControl::StoreServerHandles().
-----------------------------------------------------------------------------*/
void CMemoryMap::ReadOutput(CVARIANTArray &aValues)
{
	int i;
	VARIANT vartmp;
	VariantInit(&vartmp);
	for (i=0; i<OPCCLIENT_CONTROLFUNCTION_FLOAT_OUT_ITEM_COUNT; i++)
	{
		vartmp.vt = VT_R4;
		vartmp.fltVal = m_aFloats[i+OPCCLIENT_CONTROLFUNCTION_FLOAT_IN_ITEM_COUNT];
		aValues.Add(vartmp);
	}
	for (i=0; i<OPCCLIENT_CONTROLFUNCTION_WORD_OUT_ITEM_COUNT; i++)
	{
		vartmp.vt = VT_I2;
		vartmp.iVal = m_aWords[i+OPCCLIENT_CONTROLFUNCTION_WORD_IN_ITEM_COUNT];
		aValues.Add(vartmp);
	}
	for (i=0; i<OPCCLIENT_CONTROLFUNCTION_BYTE_OUT_ITEM_COUNT; i++)
	{
		vartmp.vt = VT_UI1;
		vartmp.bVal = m_aBytes[i+OPCCLIENT_CONTROLFUNCTION_BYTE_IN_ITEM_COUNT];
		aValues.Add(vartmp);
	}
	
}
/* ----------------------------------------------------------------------------
    Class		CMemoryMap
	Method		GetFloat
	Description	Gets a float from the member-array. Calling syntax of this 
				method is similar to SPS-logic.
	
	OPCCLIENT_CONTROLFUNCTION_FLOAT_IN_ITEM1  
	==> floatIndex	0
	
	OPCCLIENT_CONTROLFUNCTION_FLOAT_IN_ITEM2 
	==> floatIndex	4
	
	OPCCLIENT_CONTROLFUNCTION_FLOAT_OUT_ITEM1  
	==> floatIndex	4 * OPCCLIENT_CONTROLFUNCTION_FLOAT_IN_ITEM_COUNT + 4
	
	OPCCLIENT_CONTROLFUNCTION_FLOAT_OUT_ITEM2  
	==> floatIndex	4 * OPCCLIENT_CONTROLFUNCTION_FLOAT_IN_ITEM_COUNT + 8
	
-----------------------------------------------------------------------------*/
FLOAT CMemoryMap::GetInFloat(int floatIndex)
{
	ASSERT( OPCCLIENT_CONTROLFUNCTION_FLOAT_IN_ITEM_COUNT > 0 );
	ASSERT( floatIndex	>=	0 );
	ASSERT( floatIndex%4 ==	0 ); 
	ASSERT( floatIndex	<	4 * OPCCLIENT_CONTROLFUNCTION_FLOAT_IN_ITEM_COUNT);

	return m_aFloats[floatIndex/4];
}
/* ----------------------------------------------------------------------------
    Class		CMemoryMap
	Method		SetFloat
	Description	Sets the given float to the member-array. Calling syntax of this 
				method is similar to SPS-logic.

	OPCCLIENT_CONTROLFUNCTION_FLOAT_OUT_ITEM1  
	==> floatIndex	4 * OPCCLIENT_CONTROLFUNCTION_FLOAT_IN_ITEM_COUNT + 4
	
	OPCCLIENT_CONTROLFUNCTION_FLOAT_OUT_ITEM2  
	==> dwordfloatIndexIndex	4 * OPCCLIENT_CONTROLFUNCTION_FLOAT_IN_ITEM_COUNT + 8
-----------------------------------------------------------------------------*/
void CMemoryMap::SetOutFloat(int floatIndex, FLOAT value)
{
	ASSERT( OPCCLIENT_CONTROLFUNCTION_FLOAT_OUT_ITEM_COUNT > 0);
	ASSERT( floatIndex >=	0);
	ASSERT( floatIndex%4 ==	0);
	ASSERT( floatIndex <=	4 * OPCCLIENT_CONTROLFUNCTION_FLOAT_OUT_ITEM_COUNT);

	m_aFloats.SetAtGrow(floatIndex/4, value);
}
/* ----------------------------------------------------------------------------
    Class		CMemoryMap
	Method		GetWord
	Description	Gets a word from the member-array. Calling syntax of this 
				method is similar to SPS-logic.

	OPCCLIENT_CONTROLFUNCTION_WORD_IN_ITEM1  
	==> wordIndex	4 * (OPCCLIENT_CONTROLFUNCTION_FLOAT_IN_ITEM_COUNT + 
						OPCCLIENT_CONTROLFUNCTION_FLOAT_OUT_ITEM_COUNT)
	
	OPCCLIENT_CONTROLFUNCTION_WORD_IN_ITEM2  
	==> wordIndex	4 * (OPCCLIENT_CONTROLFUNCTION_BYTE_IN_ITEM_COUNT + 
						OPCCLIENT_CONTROLFUNCTION_BYTE_OUT_ITEM_COUNT) + 2
	
	OPCCLIENT_CONTROLFUNCTION_WORD_OUT_ITEM1  
	==> wordIndex	4 * (OPCCLIENT_CONTROLFUNCTION_FLOAT_IN_ITEM_COUNT + 
						OPCCLIENT_CONTROLFUNCTION_FLOAT_OUT_ITEM_COUNT) +
					2 * OPCCLIENT_CONTROLFUNCTION_WORD_IN_ITEM_COUNT
	
	OPCCLIENT_CONTROLFUNCTION_WORD_OUT_ITEM2  
	==> wordIndex	4 * (OPCCLIENT_CONTROLFUNCTION_FLOAT_IN_ITEM_COUNT + 
						OPCCLIENT_CONTROLFUNCTION_FLOAT_OUT_ITEM_COUNT) +
					2 * OPCCLIENT_CONTROLFUNCTION_WORD_IN_ITEM_COUNT + 2
	etc.
-----------------------------------------------------------------------------*/
WORD CMemoryMap::GetInWord(int wordIndex)
{
	ASSERT( OPCCLIENT_CONTROLFUNCTION_WORD_IN_ITEM_COUNT > 0 );
	ASSERT( wordIndex >= 4 * OPCCLIENT_CONTROLFUNCTION_FLOAT_IN_ITEM_COUNT);
	ASSERT( wordIndex%2 == 0 ); 
	ASSERT( wordIndex < 4 * OPCCLIENT_CONTROLFUNCTION_FLOAT_IN_ITEM_COUNT +
						2 * OPCCLIENT_CONTROLFUNCTION_WORD_IN_ITEM_COUNT);//TODO

	int index = (wordIndex - 4 * OPCCLIENT_CONTROLFUNCTION_FLOAT_IN_ITEM_COUNT) / 2;
	return m_aWords[index];
}
/* ----------------------------------------------------------------------------
    Class		CMemoryMap
	Method		SetWord
	Description	Sets the given word to the member-array. Calling syntax of this 
				method is similar to SPS-logic.

	OPCCLIENT_CONTROLFUNCTION_WORD_OUT_ITEM1  
	==> wordIndex	4 * (OPCCLIENT_CONTROLFUNCTION_FLOAT_IN_ITEM_COUNT + 
						OPCCLIENT_CONTROLFUNCTION_FLOAT_OUT_ITEM_COUNT) +
					2 * OPCCLIENT_CONTROLFUNCTION_WORD_IN_ITEM_COUNT
	
	OPCCLIENT_CONTROLFUNCTION_WORD_OUT_ITEM2  
	==> wordIndex	4 * (OPCCLIENT_CONTROLFUNCTION_FLOAT_IN_ITEM_COUNT + 
						OPCCLIENT_CONTROLFUNCTION_FLOAT_OUT_ITEM_COUNT) +
					2 * OPCCLIENT_CONTROLFUNCTION_WORD_IN_ITEM_COUNT + 2
	etc.
-----------------------------------------------------------------------------*/
void CMemoryMap::SetOutWord(int wordIndex, WORD value)
{
	ASSERT( OPCCLIENT_CONTROLFUNCTION_WORD_OUT_ITEM_COUNT > 0 );
	ASSERT( wordIndex >=4 *	OPCCLIENT_CONTROLFUNCTION_FLOAT_OUT_ITEM_COUNT);
	ASSERT( wordIndex%2 == 0 );
	ASSERT( wordIndex < 4 * OPCCLIENT_CONTROLFUNCTION_FLOAT_OUT_ITEM_COUNT +
						2 * OPCCLIENT_CONTROLFUNCTION_WORD_OUT_ITEM_COUNT);

	int index = (wordIndex - 4 * OPCCLIENT_CONTROLFUNCTION_FLOAT_OUT_ITEM_COUNT) / 2+
								OPCCLIENT_CONTROLFUNCTION_WORD_IN_ITEM_COUNT;
	m_aWords.SetAtGrow(index,value);
}
/* ----------------------------------------------------------------------------
    Class		CMemoryMap
	Method		GetByte
	Description	Gets a byte. Calling syntax of this method is similar to SPS-logic.

	OPCCLIENT_CONTROLFUNCTION_BYTE_IN1  
	==> byteIndex	4 * (OPCCLIENT_CONTROLFUNCTION_FLOAT_IN_ITEM_COUNT + 
						OPCCLIENT_CONTROLFUNCTION_FLOAT_OUT_ITEM_COUNT) +
					2 * (OPCCLIENT_CONTROLFUNCTION_WORD_IN_ITEM_COUNT + 
						OPCCLIENT_CONTROLFUNCTION_WORD_OUT_ITEM_COUNT)

	OPCCLIENT_CONTROLFUNCTION_BYTE_IN2  
	==> byteIndex	4 * (OPCCLIENT_CONTROLFUNCTION_FLOAT_IN_ITEM_COUNT + 
						OPCCLIENT_CONTROLFUNCTION_FLOAT_OUT_ITEM_COUNT) +
					2 * (OPCCLIENT_CONTROLFUNCTION_WORD_IN_ITEM_COUNT + 
						OPCCLIENT_CONTROLFUNCTION_WORD_OUT_ITEM_COUNT) + 1
	
	OPCCLIENT_CONTROLFUNCTION_BYTE_OUT1  
	==> byteIndex	4 * (OPCCLIENT_CONTROLFUNCTION_FLOAT_IN_ITEM_COUNT + 
						OPCCLIENT_CONTROLFUNCTION_FLOAT_OUT_ITEM_COUNT) +
					2 * (OPCCLIENT_CONTROLFUNCTION_WORD_IN_ITEM_COUNT + 
						OPCCLIENT_CONTROLFUNCTION_WORD_OUT_ITEM_COUNT) +
						OPCCLIENT_CONTROLFUNCTION_BYTE_IN_ITEM_COUNT
	
	
	OPCCLIENT_CONTROLFUNCTION_BYTE_OUT2  
	==> byteIndex	4 * (OPCCLIENT_CONTROLFUNCTION_FLOAT_IN_ITEM_COUNT + 
						OPCCLIENT_CONTROLFUNCTION_FLOAT_OUT_ITEM_COUNT) +
					2 * (OPCCLIENT_CONTROLFUNCTION_WORD_IN_ITEM_COUNT + 
						OPCCLIENT_CONTROLFUNCTION_WORD_OUT_ITEM_COUNT) +
						OPCCLIENT_CONTROLFUNCTION_BYTE_IN_ITEM_COUNT + 1
				
-----------------------------------------------------------------------------*/
BOOL CMemoryMap::GetInByte(int byteIndex)
{
	ASSERT(OPCCLIENT_CONTROLFUNCTION_BYTE_IN_ITEM_COUNT > 0 );
	ASSERT(byteIndex >=	4 * OPCCLIENT_CONTROLFUNCTION_FLOAT_IN_ITEM_COUNT +
						2 * OPCCLIENT_CONTROLFUNCTION_WORD_IN_ITEM_COUNT);
	ASSERT(byteIndex <	4 * OPCCLIENT_CONTROLFUNCTION_FLOAT_IN_ITEM_COUNT +
						2 * OPCCLIENT_CONTROLFUNCTION_WORD_IN_ITEM_COUNT + 
							OPCCLIENT_CONTROLFUNCTION_BYTE_IN_ITEM_COUNT);

	int index = byteIndex	- 4 * OPCCLIENT_CONTROLFUNCTION_FLOAT_IN_ITEM_COUNT 
							- 2 * OPCCLIENT_CONTROLFUNCTION_WORD_IN_ITEM_COUNT;
	return(m_aBytes[index]) ;
}
/* ----------------------------------------------------------------------------
    Class		CMemoryMap
	Method		SetByte
	Description	Sets a byte.
			
	OPCCLIENT_CONTROLFUNCTION_BYTE_OUT1  
	==> byteIndex	4 * (OPCCLIENT_CONTROLFUNCTION_FLOAT_IN_ITEM_COUNT + 
						OPCCLIENT_CONTROLFUNCTION_FLOAT_OUT_ITEM_COUNT) +
					2 * (OPCCLIENT_CONTROLFUNCTION_WORD_IN_ITEM_COUNT + 
						OPCCLIENT_CONTROLFUNCTION_WORD_OUT_ITEM_COUNT) +
						OPCCLIENT_CONTROLFUNCTION_BYTE_IN_ITEM_COUNT
-----------------------------------------------------------------------------*/
void CMemoryMap::SetOutByte(int byteIndex, BYTE value)
{
	ASSERT(OPCCLIENT_CONTROLFUNCTION_BYTE_OUT_ITEM_COUNT > 0 );
	ASSERT(byteIndex >=	4 *	OPCCLIENT_CONTROLFUNCTION_FLOAT_OUT_ITEM_COUNT +
						2 * OPCCLIENT_CONTROLFUNCTION_WORD_OUT_ITEM_COUNT);
	ASSERT(byteIndex <	4 * OPCCLIENT_CONTROLFUNCTION_FLOAT_OUT_ITEM_COUNT +
						2 * OPCCLIENT_CONTROLFUNCTION_WORD_OUT_ITEM_COUNT +
							OPCCLIENT_CONTROLFUNCTION_BYTE_OUT_ITEM_COUNT);

	int index = byteIndex	- 4 * OPCCLIENT_CONTROLFUNCTION_FLOAT_OUT_ITEM_COUNT
							- 2 * OPCCLIENT_CONTROLFUNCTION_WORD_OUT_ITEM_COUNT +
							OPCCLIENT_CONTROLFUNCTION_BYTE_IN_ITEM_COUNT 		;
	m_aBytes.SetAtGrow(index,value);
}
/* ----------------------------------------------------------------------------
    Class		CMemoryMap
	Method		GetBool
	Description	Gets a single bit from a byte. Calling syntax of this 
				method is similar to SPS-logic.

	OPCCLIENT_CONTROLFUNCTION_BYTE_IN1  
	==> byteIndex	4 * (OPCCLIENT_CONTROLFUNCTION_FLOAT_IN_ITEM_COUNT + 
						OPCCLIENT_CONTROLFUNCTION_FLOAT_OUT_ITEM_COUNT) +
					2 * (OPCCLIENT_CONTROLFUNCTION_WORD_IN_ITEM_COUNT + 
						OPCCLIENT_CONTROLFUNCTION_WORD_OUT_ITEM_COUNT)

	OPCCLIENT_CONTROLFUNCTION_BYTE_IN2  
	==> byteIndex	4 * (OPCCLIENT_CONTROLFUNCTION_FLOAT_IN_ITEM_COUNT + 
						OPCCLIENT_CONTROLFUNCTION_FLOAT_OUT_ITEM_COUNT) +
					2 * (OPCCLIENT_CONTROLFUNCTION_WORD_IN_ITEM_COUNT + 
						OPCCLIENT_CONTROLFUNCTION_WORD_OUT_ITEM_COUNT) + 1
	
	OPCCLIENT_CONTROLFUNCTION_BYTE_OUT1  
	==> byteIndex	4 * (OPCCLIENT_CONTROLFUNCTION_FLOAT_IN_ITEM_COUNT + 
						OPCCLIENT_CONTROLFUNCTION_FLOAT_OUT_ITEM_COUNT) +
					2 * (OPCCLIENT_CONTROLFUNCTION_WORD_IN_ITEM_COUNT + 
						OPCCLIENT_CONTROLFUNCTION_WORD_OUT_ITEM_COUNT) +
						OPCCLIENT_CONTROLFUNCTION_BYTE_IN_ITEM_COUNT
	
	
	OPCCLIENT_CONTROLFUNCTION_BYTE_OUT2  
	==> byteIndex	4 * (OPCCLIENT_CONTROLFUNCTION_FLOAT_IN_ITEM_COUNT + 
						OPCCLIENT_CONTROLFUNCTION_FLOAT_OUT_ITEM_COUNT) +
					2 * (OPCCLIENT_CONTROLFUNCTION_WORD_IN_ITEM_COUNT + 
						OPCCLIENT_CONTROLFUNCTION_WORD_OUT_ITEM_COUNT) +
						OPCCLIENT_CONTROLFUNCTION_BYTE_IN_ITEM_COUNT + 1
				
-----------------------------------------------------------------------------*/
BOOL CMemoryMap::GetInBool(int byteIndex, int bitIndex)
{
	ASSERT(OPCCLIENT_CONTROLFUNCTION_BYTE_IN_ITEM_COUNT > 0 );
	ASSERT(byteIndex >=	4 * OPCCLIENT_CONTROLFUNCTION_FLOAT_IN_ITEM_COUNT +
						2 * OPCCLIENT_CONTROLFUNCTION_WORD_IN_ITEM_COUNT);
	ASSERT(byteIndex <	4 * OPCCLIENT_CONTROLFUNCTION_FLOAT_IN_ITEM_COUNT +
						2 * OPCCLIENT_CONTROLFUNCTION_WORD_IN_ITEM_COUNT + 
							OPCCLIENT_CONTROLFUNCTION_BYTE_IN_ITEM_COUNT);

	int index = byteIndex	- 4 * OPCCLIENT_CONTROLFUNCTION_FLOAT_IN_ITEM_COUNT 
							- 2 * OPCCLIENT_CONTROLFUNCTION_WORD_IN_ITEM_COUNT;
	return( (m_aBytes[index] & (0x01 << bitIndex)) != 0 ) ;
}
/* ----------------------------------------------------------------------------
    Class		CMemoryMap
	Method		SetBool
	Description	Sets a single bit.
			
	OPCCLIENT_CONTROLFUNCTION_BYTE_OUT1  
	==> byteIndex	4 * (OPCCLIENT_CONTROLFUNCTION_FLOAT_IN_ITEM_COUNT + 
						OPCCLIENT_CONTROLFUNCTION_FLOAT_OUT_ITEM_COUNT) +
					2 * (OPCCLIENT_CONTROLFUNCTION_WORD_IN_ITEM_COUNT + 
						OPCCLIENT_CONTROLFUNCTION_WORD_OUT_ITEM_COUNT) +
						OPCCLIENT_CONTROLFUNCTION_BYTE_IN_ITEM_COUNT
-----------------------------------------------------------------------------*/
void CMemoryMap::SetOutBool(int byteIndex, int bitIndex, BOOL value)
{
	ASSERT(OPCCLIENT_CONTROLFUNCTION_BYTE_OUT_ITEM_COUNT > 0 );
	ASSERT(byteIndex >=	4 *	OPCCLIENT_CONTROLFUNCTION_FLOAT_OUT_ITEM_COUNT +
						2 * OPCCLIENT_CONTROLFUNCTION_WORD_OUT_ITEM_COUNT);
	ASSERT(byteIndex <	4 * OPCCLIENT_CONTROLFUNCTION_FLOAT_OUT_ITEM_COUNT +
						2 * OPCCLIENT_CONTROLFUNCTION_WORD_OUT_ITEM_COUNT +
							OPCCLIENT_CONTROLFUNCTION_BYTE_OUT_ITEM_COUNT);

	int index = byteIndex	- 4 * OPCCLIENT_CONTROLFUNCTION_FLOAT_OUT_ITEM_COUNT
							- 2 * OPCCLIENT_CONTROLFUNCTION_WORD_OUT_ITEM_COUNT;
	BYTE byte = m_aBytes[index];
	BYTE newByte = byte;
	if(value == TRUE)
		newByte = byte | (0x01 << bitIndex);//set one bit
	else
	{
		int mask = 0x01 << bitIndex;
		mask = mask ^ 0xff; // invert mask
		newByte = byte & mask; // reset one bit
	}

	m_aBytes.SetAtGrow(index,newByte);
}
/* ---------------------------------------------------------------------------
    End Class		CMemoryMap
-----------------------------------------------------------------------------*/
