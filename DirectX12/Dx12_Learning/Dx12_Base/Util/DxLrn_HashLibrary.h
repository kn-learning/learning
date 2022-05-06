#pragma once
#include "DxLrn_Utility.h"

class IDxLrnHashLibrary
{
public:
	template <class T>
	inline T* GetData(HashKey key) 
	{
		return reinterpret_cast<T*>(m_Library.at(key));
	}

	
	inline void RemoveData(HashKey key)
	{
		m_Library.erase(key);
	}

	inline size_t GetSize()
	{
		return m_Library.size();
	}

protected:

	virtual ~IDxLrnHashLibrary() 
	{
		m_Library.clear();
	}

	template <class T>
	inline T* SetData(HashKey key, T data) 
	{
		m_Library.emplece(key, data);
	}

	template <class T>
	std::unordered_map<HashKey, T> m_Library;

};