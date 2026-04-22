#include "caller.h"
#include "helpers.h"

template <typename T>
struct StringMap
{
	T* table;
	int size;
};

struct scrSignatureAndCmd
{
	uint32_t sig;
	opd_s* cmd;
};

unsigned int jenkinsHash(char* key)
{
	size_t len = _sys_strlen(key);
	unsigned int hash, i;
	for (hash = i = 0; i < len; ++i)
	{
		hash += tolower(key[i]);
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}
	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);
	return hash;
}

opd_s* LookupCommandSignature(uint32_t hashCode)
{
	StringMap<scrSignatureAndCmd>* table = reinterpret_cast<StringMap<scrSignatureAndCmd>*>(0x17D12B8);
	uint32_t index = hashCode - ((hashCode / table->size) * table->size);
	uint32_t dHash = hashCode;

	while (table->table[index].sig != hashCode)
	{
		if (table->table[index].sig == 0)
			return nullptr;

		dHash = dHash >> 1;
		dHash++;
		index += dHash;
		index -= ((index / table->size) * table->size);
	}
	return table->table[index].cmd;
}

