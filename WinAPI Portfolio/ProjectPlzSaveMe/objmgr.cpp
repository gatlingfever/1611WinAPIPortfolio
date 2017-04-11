#include "stdafx.h"

C_OBJMGR::C_OBJMGR()
	: m_ObjectSet()
{
}

//create eObjType Object (ObjType is defined in C_OBJMGR::E_ObjTYPE)
C_OBJECT* C_OBJMGR::createObj(E_ObjTYPE eObjType)
{
	C_OBJECT* pObj = new C_OBJECT;
	
	pObj->createOBJ();
	m_ObjectSet[eObjType].insert(pObj);
	return pObj;
}

//destroy pObj
objmap_citer C_OBJMGR::destroyObj(E_ObjTYPE eObjType, objmap_citer pObj)
{
	delete *pObj;
	
	return m_ObjectSet[eObjType].erase(pObj);
}

//destroy all ObjTyped Object
void C_OBJMGR::destroyAllObj(E_ObjTYPE eObjType)
{
	for (objmap_citer iter = m_ObjectSet[eObjType].begin();
		iter != m_ObjectSet[eObjType].end(); )
	{
		iter = destroyObj(eObjType, iter);
	}
	m_ObjectSet[eObjType].clear();
}

void C_OBJMGR::updateAllObj()
{
	for (int i = 1; i < E_MAX; ++i)
	{
		for (auto iter : m_ObjectSet[i])
		{
			iter->update();
		}
	}
}

//get begin iterator of eObjtype
objmap_citer C_OBJMGR::getBeginIter(E_ObjTYPE eObjType)
{
	return m_ObjectSet[eObjType].begin();
}

//get end iterator of eObjtype
objmap_citer C_OBJMGR::getEndIter(E_ObjTYPE eObjType)
{
	return m_ObjectSet[eObjType].end();
}

UINT C_OBJMGR::getSize(E_ObjTYPE eObjType)
{
	return m_ObjectSet[eObjType].size();
}
