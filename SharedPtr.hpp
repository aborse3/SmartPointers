#include<iostream>
namespace cs540{

class reference_count
{
    private:
    int c;
    public:
    void increment_Ref()
    {
      c++;
      //std::cout<<"after increament count is "<<c<<std::endl;
    }
    int decrement_Ref()
    {
      //std::cout<<"after decreament count is "<<c-1<<std::endl;
      return --c;
    }
};

class nontemplate{
public:
  nontemplate * actual_pointer;
  virtual ~nontemplate(){

  }
  virtual nontemplate &getptr(){
    return *this;
  }
  virtual int check_class(){
    return 1;
  }
};

template<typename U>
class helper:public nontemplate{
public:
  U* actual_pointer;
  helper(U* ptr){
    actual_pointer=ptr;
  }
  ~helper(){
    delete actual_pointer;
  }
  virtual helper &getptr(){
    return *this;
  }
  virtual int check_class(){
    return 2;
  }
};



template<typename T>
class SharedPtr{
public:
  reference_count * count;
  nontemplate *nt;
  T * jugad;
public:
  SharedPtr():nt(0),count(0),jugad(0){
    std::cout<<"default"<<std::endl;
    count=new reference_count();
  }


  template<typename U> explicit SharedPtr(U * obj_to_be_owned){
    std::cout<<"parameterized U ptr is assigned to T"<<std::endl;
    jugad=obj_to_be_owned;
    nt=new helper<U>(obj_to_be_owned);
    count=new reference_count();
    if(obj_to_be_owned!=NULL){
      count->increment_Ref();
    }
  }

  SharedPtr(const SharedPtr &p){
    nt=p.nt;
    jugad=p.jugad;
    p.count->increment_Ref();
    count = p.count;
    std::cout<<"copy ctor of the objects of same classes"<<std::endl;
  }

  template <typename U>
  SharedPtr(const SharedPtr<U> &p){
    std::cout<<"copy ctor of the objects of different classes"<<std::endl;
    nt=p.nt;
    jugad=p.jugad;
    p.count->increment_Ref();
    count = p.count;
  }

  SharedPtr &operator=(const SharedPtr &p){
    std::cout<<"copy assignment of the objects of same classes"<<std::endl;
    if(&p!=this){
      if(nt==NULL){
        delete count;
      }
      else if(count->decrement_Ref()==0){
        delete nt;
        delete count;
      }
      p.count->increment_Ref();
      nt=p.nt;
      jugad=p.jugad;
      count=p.count;
    }
    return *this;
  }

  template <typename U>
  SharedPtr<T> &operator=(const SharedPtr<U> &p){
    std::cout<<"copy assignment of the objects of different classes"<<std::endl;
    auto i=count->decrement_Ref();
      if(i==-1){
        delete count;
      }
      else if(i==0){
        delete nt;
        delete count;
      }
      p.count->increment_Ref();
      nt=p.nt;
      jugad=p.jugad;
      count=p.count;
      return *this;
  }

  SharedPtr(SharedPtr &&p){
    std::cout<<"Move copy constructor of same objects "<<std::endl;
    if(&p!=this){
      if(nt==NULL){
        delete count;
      }
      else if(count->decrement_Ref()==0){
        delete nt;
        delete count;
      }
      nt=p.nt;
      jugad=p.jugad;
      count=p.count;
      p.nt=NULL;
      p.jugad=NULL;
      p.count=NULL;
    }
  };

  template <typename U>
  SharedPtr(SharedPtr<U> &&p){
    std::cout<<"Move copy constructor of different objects"<<std::endl;
    if(&p!=this){
      if(nt==NULL){
        delete count;
      }
      else if(count->decrement_Ref()==0){
        delete nt;
        delete count;
      }
      nt=p.nt;
      jugad=p.jugad;
      count=p.count;
      p.nt=NULL;
      p.jugad=NULL;
      p.count=NULL;
    }

  };

  SharedPtr &operator=(SharedPtr &&p){
    std::cout<<"Move = assignment"<<std::endl;
    if(&p!=this){
      if(nt==NULL){
        delete count;
      }
      else if(count->decrement_Ref()==0){
        delete nt;
        delete count;
      }
      nt=p.nt;
      jugad=p.jugad;
      count=p.count;
      p.nt=NULL;
      p.jugad=NULL;
      p.count=NULL;
    }
    return *this;
  };

  template <typename U> SharedPtr &operator=(SharedPtr<U> &&p){
    if(&p!=this){
      if(nt==NULL){
        delete count;
      }
      else if(count->decrement_Ref()==0){
        delete nt;
        delete count;
      }
      nt=p.nt;
      jugad=p.jugad;
      count=p.count;
      p.nt=NULL;
      p.jugad=NULL;
      p.count=NULL;
    }
    return *this;
  };

  void reset(){
    auto i=count->decrement_Ref();
    if(i==0){
      delete count;
      delete nt;
    }
    else if(i==-1){
      delete count;
    }
    count = new reference_count();
    nt=NULL;
    jugad=NULL;
  }

  template <typename U>
  void reset(U *p){
    auto i=count->decrement_Ref();
    if(i==0){
      delete count;
      delete nt;
    }
    else if(i==-1){
      delete count;
    }
    count= new reference_count();
    count->increment_Ref();
    nt=new helper<U>(p);
    jugad=p;
  }

  T *get() const{
    return jugad;
  }

  T &operator*() const{
    return *jugad;
  }

  T *operator->() const{
    return jugad;
  }

  explicit operator bool() const{
    if(nt!=NULL)
      return true;
    else
      return false;
  }


  ~SharedPtr(){
    if(count!=NULL&&nt!=NULL){
      if(count->decrement_Ref()==0){
        delete count;
        std::cout<<"deleting the object "<<std::endl;
        delete nt;
      }

    }
    else{
      std::cout<<"bhosadika "<<std::endl;
      delete count;
    }

    std::cout<<"destructor"<<std::endl;
  }



};


  template <typename T, typename U>
  SharedPtr<T> dynamic_pointer_cast(const SharedPtr<U> &sp) {
    SharedPtr<T> s;// = dynamic_cast<SharedPtr<T>>(sp);
    s.jugad=dynamic_cast<T*>(sp.jugad);
    s.nt=dynamic_cast<helper<U>*>(sp.nt);
    s.count=sp.count;
    return s;
  }

  template <typename T, typename U>
  SharedPtr<T> static_pointer_cast(const SharedPtr<U> &sp) {
    SharedPtr<T> s;// = dynamic_cast<SharedPtr<T>>(sp);
    s.jugad=static_cast<T*>(sp.jugad);
    s.nt=static_cast<helper<U>*>(sp.nt);
    s.count=sp.count;
    return s;
  }

  template <typename T1, typename T2>
  bool operator==(const SharedPtr<T1> &sp1, const SharedPtr<T2> &sp2){
    if(sp1.jugad==NULL&&sp2.jugad==NULL){
      return true;
    }
    else if(sp1.jugad==sp2.jugad){
      return true;
    }
    else{
      return false;
    }
  }

  template <typename T>
  bool operator==(const SharedPtr<T> &sp, std::nullptr_t){
    if(sp.jugad==NULL){
      return true;
    }
    else{
      return false;
    }
  }

  template <typename T>
  bool operator==(std::nullptr_t, const SharedPtr<T> &sp){
    if(sp.jugad==NULL){
      return true;
    }
    else{
      return false;
    }
  }

  template <typename T1, typename T2>
  bool operator!=(const SharedPtr<T1>&sp1, const SharedPtr<T2>sp2 ){
    if(sp1.jugad==NULL&&sp2.jugad!=NULL){
      return true;
    }
    else if(sp1.jugad==NULL&&sp2.jugad!=NULL){
      return true;
    }
    else if(sp1.jugad==sp2.jugad){
      return true;
    }
    else{
      return false;
    }
  }

  template <typename T>
  bool operator!=(const SharedPtr<T> &sp, std::nullptr_t){
    if(sp.jugad!=NULL){
      return true;
    }
    else{
      return false;
    }
  }

  template <typename T>
  bool operator!=(std::nullptr_t, const SharedPtr<T> &sp){
    if(sp.jugad!=NULL){
      return true;
    }
    else{
      return false;
    }
  }


}
