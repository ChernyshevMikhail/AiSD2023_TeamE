#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>


#include <vector>
#include <cassert>
#include <initializer_list>



class LongValue{
    private:
        int *values;      // массив с цифрами числа записанными в обратном порядке
        size_t length;    // длинна числа
        int sign;
        int LenghtInt(int t){
            int res=0;
            while(t!=0){
                res++;  
                t=t/10;
            }
            
            return res;
        }
    public:
        LongValue() : values(nullptr), length(0), sign(1) {}  // конструктор по умолчанию

        LongValue(int *x, size_t l,int s=1 ) : values(new int[l]), length(l), sign(s) {
            
            for (size_t i = 0; i < length; ++i) {
                values[i] = x[i];
            } 
        }
        
        LongValue(int x){
            length=LenghtInt(x);
            values = new int[length];

            if(x<0){sign=-1;}
            else{sign=1;}

            for (size_t i = 0; i < length; ++i) {
                values[i] = (x%10)*sign;
                x=x/10;
            } 
        }
        LongValue(const LongValue& other) : length(other.length), sign(other.sign) {
            values = new int[length];
            for (size_t i = 0; i < length; ++i) {
                values[i] = other.values[i];
            }
        }
        LongValue(const std::initializer_list<int>& ilist, int s = 1) : length(ilist.size()), sign(s) {
            values = new int[length];
            size_t i = 0;
            for (int value : ilist) {
                values[i++] = value;
            }
        }
        LongValue& operator=(const LongValue& a) {
            if (this != &a) {  
                delete[] values;  

                length = a.length;
                sign = a.sign;
                values = new int[length];

                for (size_t i = 0; i < length; ++i) {
                    values[i] = a.values[i];
                }
            }
            return *this;
        }

        // Деструктор для освобождения выделенной памяти
        ~LongValue() {
            delete[] values;
        }
        size_t getLength() const{
            return length;
        }
        void setSign(int i){
            sign = i;
        }
        LongValue Module() const{
            LongValue res(values,length,1);
            return res;
        }
        size_t CountLeadingZeros(const LongValue& a) const{
            size_t res=0;
            for(size_t i = a.getLength()-1; i>=0; i--){
                //std::cout<<result[i];
                if(a[i]==0 and i>0){res++;}
                else{break;}
            }
            return res;
        }
        
        // Оператор сложения
        LongValue operator+(const LongValue& other) const {
            size_t maxLength = std::max(length-CountLeadingZeros(*this), other.length-CountLeadingZeros(other));
            
            int* result = new int[maxLength + 1]; // +1 на случай, если сложение вызовет перенос разряда
            std::memset(result, 0, sizeof(int) * (maxLength + 1)); // Инициализация массива результата нулями
            int s=1;
            for (size_t i = 0; i < maxLength; ++i) {
                int digitSum = result[i];
                
                // Если есть цифра в текущем объекте, добавляем её
                if (i < length) {
                     digitSum += values[i]*sign;
                }
                    // Если есть цифра в другом объекте, добавляем её
                if (i < other.length) {
                    digitSum += other.values[i]*other.sign;
                }
                result[i]=digitSum;
            }
            
            for(size_t i=0; i<maxLength; i++) {
                if(result[i]<0){
                    result[i]=result[i]+10;
                    result[i+1]=result[i+1]-1;
                }
                
                std::cout << result[i] << ' ';
            }
            std::cout << std::endl;
            
            // Проверка на ведущий ноль
            size_t resultLength=maxLength+1;
            for(size_t i=resultLength-1; i>=0; i--){
             
                if(result[i]==0 and i>0){resultLength--;}
                else{break;}
            }

            if(result[resultLength-1]<0){s=-1;}
            else{s=1;} 
            
            
            for(size_t i=resultLength;i>0;i--){
                result[i-1]=abs(result[i-1]);
            }
            // Создание нового объекта LongValue с результатом сложения
            LongValue sum(result, resultLength, s);

            // Освобождение выделенной памяти
            delete[] result;

            return sum;
        }
        LongValue operator-(const LongValue& other) const {
            
            LongValue res(other.values, other.length, other.sign*-1);
            res=res+(*this);
            std::cout<<*(this)<<" - "<<other<<" = "<<res<<'\n';
            return res;
        }

        
        // Оператор сравнения
        bool operator==(const LongValue& other) const {
            size_t l1=length-CountLeadingZeros(*this);
            size_t l2=other.length-CountLeadingZeros(other);
            if (l1 != l2) {
                return false;
            }
            if(sign != other.sign){
                for (size_t i = 0; i < l1; ++i) {
                    if ((values[i] != other.values[i]) or values[i]!=0) {
                        return false;
                    }
                }
            }
            else{
                for (size_t i = 0; i < l1; ++i) {
                    if (values[i] != other.values[i]) {
                        return false;
                    }
                }
            }
            return true;
        }

        bool operator>(const LongValue& other) const {
            size_t l1=length-CountLeadingZeros(*this);
            size_t l2=other.length-CountLeadingZeros(other);

            if (sign!=other.sign) {
                return sign>other.sign;
            }
            else{
                if(l1 < l2) {
                    return (sign-1)!=0;
                }
                else if(l1 > l2){
                    return (sign-1)==0;
                }
                for (size_t i = l1-1; i>0 ; i--) {
                    if (values[i]>other.values[i]) {
                        return true;
                    }
                }
                return false;    
            }
        }
        bool operator<(const LongValue& other) const {
            size_t l1=length-CountLeadingZeros(*this);
            size_t l2=other.length-CountLeadingZeros(other);

            if (sign!=other.sign) {
                return sign<other.sign;
            }
            else{
                if(l1 < l2) {
                    return (sign-1)==0;
                }
                else if(l1 > l2){
                    return (sign-1)!=0;
                }
                for (size_t i = l1-1; i>0 ; i--) {
                    if (values[i]<other.values[i]) {
                        return true;
                    }
                }
                return false;
            }
        }

        bool operator>=(const LongValue& other) const {
            return !(*(this)<other);
        }
        bool operator<=(const LongValue& other) const {
            return !(*(this)>other);
        }
        

        int operator[](size_t index) const {
            if (index < length) {
                return values[index];
            } 
            else { return -1; }
        }
        LongValue Slice(size_t n, size_t m, bool ZeroIgnore = false) {
            size_t l = (*this).length;
            if (ZeroIgnore) {
                l = l - CountLeadingZeros(*this);
            }
            if (m + n <= l) {
                LongValue res((*this).values + m, l - m - n, (*this).sign);
                return res;
                }
            else {
                LongValue res(nullptr, 0, (*this).sign);  // You might want to reconsider passing nullptr
                return res;
            }
        }
        void MakeEvenLength(){
            if((*this).length%2!=0){
                (*this)=LongValue((*this).values,(*this).length+1,(*this).sign);
            }
        }
        void SetLength(size_t n){
            if(length<n){
                int* v = new int[n];
                for (size_t i = 0; i < n; ++i) {
                    if(i<length){
                       v[i]=values[i]; 
                    }
                    else{
                        v[i] = 0;
                    }
                }
                
                *this=LongValue(v,n,sign);
            }
        }
        LongValue AddZero(size_t n){
            int* v = new int[length+n];

                for (size_t i = 0; i < length+n; ++i) {
                    if(i<n){
                       v[i]=0; 
                    }
                    else{
                        v[i] = (*this).values[i-n];
                    }
                }
            return LongValue(v,length+n,sign);
            //delete[] values;
            
            
        }
        int GetInt(){
            size_t l = length - CountLeadingZeros(*this);
            int res=0;
            if(l<10){
                for(int i=0;i<l;i++){
                    res=res+values[i]*(pow(10,i));
                }
            }
            return res*sign;
        }
        LongValue operator*(const LongValue& other) const {
            
            int s=(*this).sign*(other.sign);
            LongValue A=(*this);
            LongValue B=other;
            size_t M = std::max(A.getLength(),B.getLength());
            if(M%2!=0){M=M+1;}
            A.SetLength(M);
            B.SetLength(M);
           
           if(M<=4){
            return LongValue(A.GetInt()*B.GetInt());
           }
           else{

            LongValue A1=A.Slice(0,A.getLength()/2);
            LongValue A2=A.Slice(A.getLength()/2,0);
            LongValue B1=B.Slice(0,B.getLength()/2);
            LongValue B2=B.Slice(B.getLength()/2,0);

            std::cout<<M<<'\n';
            std::cout<<A1<<" "<<A2<<'\n';
            std::cout<<B1<<" "<<B2<<'\n';

            LongValue P1 = (A1 * B1);
            std::cout<<"P1 = "<<P1<<'\n';
            LongValue P2 = (A2 * B2);
            std::cout<<"P2 = "<<P2<<'\n';
            LongValue P3 = (A1 + A2) * (B1 + B2);
            std::cout<<"P3 = "<<P3<<'\n';
            
            LongValue res = P1.AddZero(M)+(P3-P1-P2).AddZero(M/2)+P2;
            //std::cout<<P1.AddZero(M)<<" + "<<(P3-P1-P2).AddZero(M/2)<<" + "<<P2<<" = "<<res<<'\n';
            
            return res;
           }
        }

        

        friend std::ostream& operator<<(std::ostream& os, const LongValue& value) {
            if(value.sign==1){ os << "+";}
            if(value.sign==-1){ os << "-";}
            for (size_t i = value.length; i > 0; --i) {
                os << value.values[i - 1];
            }
            return os;
        }

        
};



void Test(LongValue& res, LongValue& answer, bool check=true){
    std::cout<< "res: " << res <<", "<< "answer: "<<answer;
    if((res == answer)==check){ 
        std::cout<<" Ok";
    }
    else{
        std::cout<<" Err";
    }
    std::cout<<'\n';
}


int main() {
    /*
    LongValue a1({4,5,3,0,0,0,0},1);
    LongValue la1({4,5,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},1);
    LongValue nla1({4,5,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},-1);
    LongValue na1({4,5,3},-1);

    LongValue b1({4,7,3,3,6},1);
    LongValue nb1({4,7,3,3,9},-1);

    LongValue d1({4},1);
    LongValue nd1({4},-1);

    LongValue s1({9,7},1);
    LongValue ns1({9,7},-1);

    LongValue q1({0,0,0,0,0,0,0,1},1);
    LongValue q2({1,0,0,0,0,0,0,1},-1);

    LongValue resq=q2+q1;
    std::cout<<resq<<std::endl;

    std::cout << (a1>a1==0)<<'\n';
    std::cout << (na1>nla1==0)<<'\n';
    std::cout << (na1<nla1==0)<<'\n';
    std::cout << (a1>na1==1)<<'\n';
    std::cout << (a1>s1==1)<<'\n';
    std::cout << (d1<s1==1)<<'\n';
    std::cout << (nd1<na1==0)<<'\n';
    std::cout << '\n';

    std::cout << (a1>=a1==1)<<'\n';
    std::cout << (na1>=nla1==1)<<'\n';
    std::cout << (na1<=nla1==1)<<'\n';
    std::cout << (a1>=na1==1)<<'\n';
    std::cout << (a1>=s1==1)<<'\n';
    std::cout << (d1<=s1==1)<<'\n';
    std::cout << (nd1<=na1==0)<<'\n';
    std::cout << '\n';
    Test(a1,a1,1);
    Test(la1,a1,1);
    Test(ns1,ns1,1);
    Test(a1,na1,0);
    Test(a1,ns1,0);
    std::cout << '\n';
    LongValue res1=a1+a1;
    LongValue answer1({8,0,7},1);
    Test(res1, answer1,1);

    LongValue res2=a1+s1;
    LongValue answer2({3,3,4},1);
    Test(res2, answer2,1);

    LongValue res3=a1+na1;
    LongValue answer3({0},1);
    Test(res3, answer3,1);

    LongValue res4=s1+nb1;
    LongValue answer4({5,9,2,3,9},-1);
    Test(res4, answer4,1);

    LongValue res7=nb1+nb1;
    LongValue answer7({8,4,7,6,8,1},-1);
    Test(res7, answer7,1);
    */
    //LongValue g({2,3,4,5,6},1);
    //LongValue gg(-12323);
    //std::cout<<gg<<'\n';
    //std::cout<<gg.GetInt()<<'\n';
    //gg.SetLength(13);
    //std::cout<<gg<<'\n';
    //std::cout<<gg.getLength()<<'\n';
    //gg.MakeEvenLength();
    //std::cout<<gg<<'\n';
    //std::cout<<gg.getLength()<<'\n';
    //std::cout<<g.Slice(1,2,false) << '\n';
    //std::cout<<g.Slice(2,2) << '\n';
    LongValue a(12366);
    LongValue b(3664);
    //std::cout<<a*b;
    LongValue g(252126);
    LongValue h(-36);
    std::cout<<g+h;

/*
    LongValue res5=nb1+d1;
    LongValue answer5({0,7,3,3,6},-1);
    Test(res5, answer5,1);

    LongValue res6=nb1+nla1;
    LongValue answer6({8,2,7,3,6},-1);
    Test(res6, answer6,1);
*/
    
    return 0;
}
