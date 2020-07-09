/*************************************************************************
CULFS - C++ UTILITIES LIBRARY FOR STRINGS
Author...............: Fabio Liguori
Version..............: 2020.07.u0 (Codename: Ice Cube)

(C) Fabio Liguori, 2020, GNU General Public License v3.0

Note: this library will frequently be updated, so stay tuned!

See more on github.com/LigFabio/CULFS
*************************************************************************/

#ifndef CULFS_H_INCLUDED
#define CULFS_H_INCLUDED

#include <iostream>

#include <string.h>

#define ELEMENT_NOT_FOUND -968261825
#define APPEND_BEGINNING 0
#define APPEND_END -3
#define COUNTDEC_ERROR -1000

using namespace std;

class StringToolkit {
    private:
        int reach0(int);
        double pot(double, int);
        int countdec(char*);

    public:
        char* numtostring(double, char*&, int = 0, double = 0, double = 0, bool = false);
        double stringtonum(char*, int = -1, int = -1);
        int search_in_string(const char*, char*);
        bool is_a_number(char*, int = -1);
        bool append_to_string(char*&, const char*, int = APPEND_END);
        bool repairstring(char*&);
};

int StringToolkit::reach0 (int n) {
    if (n!=0) return 1+this->reach0(n/10);
    else return -1;
}

double StringToolkit::pot(double n, int exp) {
    if (exp>0) {return n*this->pot(n,exp-1);}
    else if (exp<0) {return n*this->pot(n,exp+1);}
    else return 1;
}

int StringToolkit::countdec(char* n) {
    int l = strlen(n)-1;
    char backup [l+1];
    strcpy(backup,n);
    if (n && l>0) {
        if (backup[l]=='.') return 0;
        else {
            backup[l] = '\0';
            return 1+this->countdec(backup);
        }
    }
    else return COUNTDEC_ERROR;
}

char* StringToolkit::numtostring (double n, char*& buf, int sp /*start point*/, double maxpos, double original_n, bool decm) {
    ///Description:
    ///Will convert a decimal number to a string containing the number (the digits will be turned into the corresponding characters).
    ///ONLY SET THE FIRST TWO PARAMETERS!

    //Implementation
    if (sp==0&&buf==NULL) {
        if (n!=(int)n) decm = true;

        buf = new char [40];
        maxpos = pot(10,this->reach0(n));

        original_n = n;
        int nint = n; n = nint; //avoids the casting's approximation and allows the program to use the int's type rules even if a double is being converted
    } //
    if (maxpos<1) {
        if (decm) {
            buf[sp] = '.';
            int original_n_int = original_n;
            n = original_n - original_n_int; //removes the integer part which has been already converted
            if (n<0) n*=-1;
            n*=this->pot(10,6);
            maxpos = this->pot(10,5);
            return this->numtostring(n,buf,sp+1,maxpos,original_n,false);
        }
        else {
            buf[sp] = '\0';
            return buf;
        }
    }
    else if (n<0&&sp==0) {
        buf[0] = '-';
        n*=(-1);
        maxpos*=10; //adds one position because of the minus
    }
    else {
        buf[sp] = '0' + (int)(n/maxpos);
        n -= ((int)(n/maxpos))*maxpos;
    }
    return this->numtostring(n,buf,sp+1, maxpos/10,original_n,decm);
} //

double StringToolkit::stringtonum(char* n, int sp /*start point*/, int len) {
    ///Description:
    ///Converts a string containing exclusively a number to an actual decimal number, returned by the function itself. In case of error, the function will display a message.
    ///ONLY SET THE FIRST PARAMETER!
    //Implementation
    int part = 0; //a partial value for the debug in the first else
    if (!n || !this->is_a_number(n)) {
        printf("Invalid parameter provided.\n");
        return 0;
    }
    if (len < 0) len = strlen(n)-1;
    if (len>0||sp<0) {
        if (sp<0) {
            int s = 1;
            if (n[0]=='-') {s = -1; len--; sp++;}
            int cd = this->countdec(n);
            if (cd<0) {
                cd = 0;
                len++;
            }
            double partial = s*(1/this->pot(10, cd));
            return partial*(this->stringtonum(n,sp+1,len));
        }
        else if (n[sp]=='\0') return 0;
        else if (n[sp]!='.' && n[sp]!=','){
            int pt = this->pot(10,len-1);
            part = (n[sp]-'0')*pt;
            len--;
        }
        return part+(this->stringtonum(n,sp+1,len));
    }
    else return 0;
}

int StringToolkit::search_in_string(const char* to_search, char* source) {
    ///Description:
    ///Searches a fragment of a specified string is actually contained in it.
    ///If it isn't, a negative value is returned.

    //Implementation
    if (strlen(source)<=0 || !source) return ELEMENT_NOT_FOUND;
    const int len = strlen(to_search);
    char buf [len+1];
    for (int i=0; i<len; i++) buf[i] = source[i];
    buf[len] = '\0';
    if (!strcmp(to_search, buf)) return 0;
    else return 1+this->search_in_string(to_search, source + 1);
}

bool StringToolkit::append_to_string (char*& source, const char* to_append, int pos) {
    ///Description:
    ///Appends a string to another one on a specified position.
    ///To append the second string to the first one, set the last parameter to APPEND_END; to append it at the beginning of the first one, set it to APPEND_BEGINNING; otherwise, specify a position.

    //Implementation
    if (!source || !to_append) return false;
    if (pos == APPEND_END) pos = strlen(source);

    int len = strlen(source)+1;
    char tmp [len];
    strcpy(tmp, source);
    tmp[len-1] = '\0';
    int applen = strlen(to_append);
    int newlen = len + applen + 1;

    source = new char [newlen];
    for (int i=0, k=0; i<newlen; i++) {
        if (i==newlen-1) source[i] = '\0';
        else if (i>=pos && k<applen) source[i] = to_append[k++];
        else source[i] = tmp [i-k];
    }
    return true;
}

bool StringToolkit::is_a_number (char* s, int len) {
    ///Description:
    ///Checks if a string contains exclusively a number.

    //Implementation
    if (len<0) {
        if (!(len = strlen(s))) return false;
    }
    if (len) {
        if ((s[len-1] >= '0' && s[len-1] <= '9') || s[len-1] == '.' || s[len] == ',') return true*is_a_number(s, len-1);
        else return false;
    }
    return true;
}

bool StringToolkit::repairstring(char*& str) {
    ///Description:
    ///Repairs potentially corrupted characters and restores their appropriate ASCII value. The function returns false if the provided parameter is not valid, otherwise it returns true.

    //Implementation
    if (!str) return false;
    else if (strlen(str)<=0) return true;

    if (*str==-24) *str = 138;
    else if (*str==-23) *str = 130;
    else if (*str==-25) *str = 135;
    else if (*str==-13) *str = 149;
    else if (*str==-32) *str = 133;
    else if (*str==-80) *str = 248;
    else if (*str==-89) *str = 245;
    else if (*str==-7) *str = 151;
    else if (*str==-93) *str = 156;
    else if (*str==-20) *str = 141;
    char* tmp = str + 1;
    return repairstring(tmp);
}

#endif // CULFS_H_INCLUDED
