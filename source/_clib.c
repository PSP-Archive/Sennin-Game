// clib-mimic


#include "_clib.h"


// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void _strcpy(char *d,const char *s)
{
	for (; *s!=0; ++s, ++d) *d=*s;
	*d=0;
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void _strcat(char *d,const char *s)
{
	for (; *d!=0; ++d) ;
	for (; *s!=0; ++s, ++d) *d=*s;
	*d=0;
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void _strncpy(char *d,const char *s,int n)
{
	for (; *s!=0 && n>0; ++s, ++d, --n) *d=*s;
	*d=0;
}


// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
char* _strchr(const char *_s, int c)
{
	char *s=(char *)_s;
	for (; *s!=0; s++) {
		if (*s==(char)c) return s;
	}
	return NULL;
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
unsigned int _strlen(const char *s) 
{
	unsigned int r=0;
	for (; *s!=0; ++s, ++r) ;
	return r;
}


// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void _memset(void *d, const long s, unsigned long n)
{
	for (; n>0; n--) *(((char *)d)++)=s;
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
int _memcmp(const void *s1, const void *s2, unsigned long n)
{
	for (; n>0; n--) {
		if (*(((unsigned char *)s1))>*(((unsigned char *)s2))) return 1;
		if (*(((unsigned char *)s1)++)<*(((unsigned char *)s2)++)) return -1;
	}
	return 0;
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void _memcpy(void *d, const void *s, unsigned long n)
{
	for (; n>0; n--) {
		(*(((unsigned char *)d)++)=*(((unsigned char *)s)++));
	}
}


// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void _strrev(char *s)
{
	char tmp;
	int i;
	int len = _strlen(s);
	
	for(i=0; i<len/2; i++){
		tmp = s[i];
		s[i] = s[len-1-i];
		s[len-1-i] = tmp;
	}
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void _ustoa(unsigned short val, char *s) {
	char *t;
	unsigned short mod;
	
	t = s;
	
	while(val) {
		mod = val % 10;
		*t++ = (char)mod + '0';
		val /= 10;
	}

	if(s == t)
		*t++ = '0';

	*t = '\0';

	_strrev(s);
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void _ultoa(unsigned long val, char *s) {
	char *t;
	unsigned long mod;
	
	t = s;
	
	while(val) {
		mod = val % 10;
		*t++ = (char)mod + '0';
		val /= 10;
	}

	if(s == t)
		*t++ = '0';

	*t = '\0';

	_strrev(s);
}
