#pragma once

#include "GameInfo.h"

class CRef
{
public:
	CRef();
	virtual ~CRef();

protected:
	std::string m_Name;
	int		m_RefCount;
	bool	m_Enable;
	bool	m_Active;
	size_t	m_TypeID;

public:
	void Enable(bool bEnable)
	{
		m_Enable = bEnable;
	}

	void Destroy()
	{
		m_Active = false;
	}

	bool IsActive()	const
	{
		return m_Active;
	}

	bool IsEnable()	const
	{
		return m_Enable;
	}

	void AddRef()
	{
		++m_RefCount;
	}

	int GetRefCount()	const
	{
		return m_RefCount;
	}

	int Release()
	{
		--m_RefCount;

		if (m_RefCount <= 0)
		{
			delete this;
			return 0;
		}

		return m_RefCount;
	}

	void SetName(const std::string& Name)
	{
		m_Name = Name;
	}

	const std::string& GetName()	const
	{
		return m_Name;
	}

	template <typename T>
	void SetTypeID()
	{
		// 동일한 type을 참조하는 모든 type_info 객체의 hash_code()가 동일하도록
		// 지정되지 않은 값을 반환한다.
		m_TypeID = typeid(T).hash_code();
	}

	template <typename T>
	bool CheckType()
	{
		return m_TypeID == typeid(T).hash_code();
	}
};

