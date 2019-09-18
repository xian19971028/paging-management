# include <stdio.h>
# include <stdlib.h>
# include <iostream>
# include <fstream>
# include <string.h>
# include <iomanip>
# include <vector>
# include <cmath>

using namespace std;

FILE *fp, *fout;

char name[50];
char outputname[50];
int pageframenum = 0 ;

class Data {
  public:
    int num = 0 ;
    int time = 0 ;
};

class Use{
  public:
    int counter = 0;
    int num = -1;
    int time = -1 ;
};

class Additional{
  public:
    int shifter = 0 ;
    int bit = 0;
    int num = -1;
};

class Result{
  public:
    int num = 0;
    bool pagefault = false ;
    vector<int> pageframeuse;
};

vector<Result> result;
vector<Data> Inputdata;
vector<int> pageframe;



int Input(){
  int num, eof;
  string inputname, filename, outputfile;
  
  cout << "Enter filename ....Enter 0 to exit" << endl;
  cin >> inputname;
  if(inputname == "0")
    return 1;
    
  filename = inputname + ".txt";
  outputfile = inputname + "_output.txt";
  
  strcpy(name, filename.c_str());
  strcpy(outputname, outputfile.c_str());
  
  fp = fopen(name, "rt");
  
  while( !fp ){
    cout << "*** " << name << " dose not exist! ***" << endl;
    cout << "please enter filename" << endl;
    cin >> inputname;
    
    filename = inputname + ".txt";
    outputfile = inputname + "_output.txt";
    strcpy(name, filename.c_str());
    strcpy(outputname, outputfile.c_str());
  
    fp = fopen(name, "rt");
  }
  
  fout = fopen(outputname,"w+t");
  eof = fscanf(fp, "%d", &pageframenum);
  
  char pagenum;
  int time = -1 ;
  
  while( eof != -1 ){
    eof = fscanf(fp, "%c ", &pagenum);
    if( eof == -1 )
      break;
    
    Data temp;
    
    temp.num = pagenum - '0';
    temp.time = time;
    //cout << pagenum << " " << time << endl; 
    Inputdata.push_back(temp);
    time++;
  }
  
  return 0 ;
}

bool ispagefault( int num ){
  for( int i = 0 ; i < pageframenum ; i++ ){
    if( i >= pageframe.size() ) 
      break;
    else{
      if( pageframe[pageframe.size()-i-1] == num )
        return false;
    }
  }
  return true;
}


void FIFO(){
  Result temp;
  
  for( int i = 0 ; i < Inputdata.size() ; i++ ){

    temp.num = Inputdata[i].num;
    if( ispagefault(temp.num) )
      temp.pagefault = true;
    else 
      temp.pagefault = false;
    
    if ( temp.pagefault == true )
      pageframe.push_back( temp.num ) ;
    
    vector<int> temp_pageframe;

    for( int i = 0 ; i < pageframenum ; i++ ){
      if( i >= pageframe.size() ) 
        break;
      else{
        temp_pageframe.push_back(pageframe[pageframe.size()-i-1]) ;
      }
    }
    
    temp.pageframeuse = temp_pageframe ;
    result.push_back(temp);
    temp_pageframe.clear();
  }
}

void LRU(){
  Result temp;
  
  for( int i = 0 ; i < Inputdata.size() ; i++ ){

    temp.num = Inputdata[i].num;
    if( ispagefault(temp.num) )
      temp.pagefault = true;
    else 
      temp.pagefault = false;
    
    if( temp.pagefault == true )
      pageframe.push_back( temp.num ) ;
    else{
      if( pageframe.size() == 1 ){
        if( pageframe[0] == temp.num )
          ;
      }
      else if( pageframe.size() == 2 ){
        if( pageframe[1] == temp.num )
          ;
        else if( pageframe[0] == temp.num )
          swap( pageframe[0], pageframe[1] );
      }
      
      else if( pageframe.size() >= 3 ){
        int j = 0 ;
        for( j ; j < pageframenum ; j++ ){
          if( j >= pageframe.size() ) 
            break;
          else{
            if( pageframe[pageframe.size()-j-1] == temp.num )
              break;
          }
        }

        int tempint = pageframe[pageframe.size()-j-1];
        for( int i = pageframe.size()-j-1 ; i < pageframe.size()-1 ; i++ ){
          pageframe[i] = pageframe[i+1] ;
        }
        
        pageframe[pageframe.size()-1] = tempint;
      }
    }

    vector<int> temp_pageframe;
    for( int i = 0 ; i < pageframenum ; i++ ){
      if( i >= pageframe.size() ) 
        break;
      else{
        temp_pageframe.push_back(pageframe[pageframe.size()-i-1]) ;
      }
    }

    temp.pageframeuse = temp_pageframe ;
    result.push_back(temp);
    temp_pageframe.clear();
  }
}

int min( vector<Use> pageFrameUseTime ){
  int minnum = 0 ;
  vector<Use>temp = pageFrameUseTime ;
  
  for( int i = 0 ; i < temp.size() ; i++ ){
    for( int j = i + 1 ; j < temp.size() ; j++ ){
      if( temp[j].counter < temp[i].counter ){
        swap( temp[j], temp[i] ) ;
      }
    }
  }
  
  for( int j = 0 ; j < temp.size() ; j++ ){
    if( temp[0].counter == pageFrameUseTime[j].counter ){
      minnum = j ;
      return minnum;
    }
  }
  
  //return minnum;
}

int min_scp( vector<Use> pageFrameUseTime ){
  int minnum = 0 ;
  vector<Use>temp = pageFrameUseTime ;
  
  for( int i = 0 ; i < temp.size() ; i++ ){
    for( int j = i + 1 ; j < temp.size() ; j++ ){
      if( temp[j].time < temp[i].time ){
        swap( temp[j], temp[i] ) ;
      }
    }
  }
  

  for( int j = 0 ; j < temp.size() ; j++ ){
    if( temp[0].time == pageFrameUseTime[j].time ){
      minnum = j ;
      return minnum;
    }
  }
  
  //return minnum;
}

int min_scp2( vector<Use> &pageFrameUseTime ){
  int minnum = 0 ;
  vector<Use>temp = pageFrameUseTime ;
  
  for( int i = 0 ; i < temp.size() ; i++ ){
    for( int j = i + 1 ; j < temp.size() ; j++ ){
      if( temp[j].time < temp[i].time ){
        swap( temp[j], temp[i] ) ;
      }
    }
  }
  
  
  for( int j = 0 ; j < temp.size() ; j++ ){
    if( temp[j].counter == 0 ){
      minnum = j ;
      return minnum;
    }
  }
  
  //return minnum;
}

int max( vector<Use> pageFrameUseTime ){
  int maxnum = 0 ;
  vector<Use>temp = pageFrameUseTime ;
  
  for( int i = 0 ; i < temp.size() ; i++ ){
    for( int j = i + 1 ; j < temp.size() ; j++ ){
      if( temp[j].counter > temp[i].counter ){
        swap( temp[j], temp[i] ) ;
      }
    }
  }
  
  for( int j = 0 ; j < temp.size() ; j++ ){
    if( temp[0].counter == pageFrameUseTime[j].counter ){
      maxnum = j ;
      return maxnum;
    }
  }
  
  //return minnum;
}
void LFU(){
  Result temp;
  vector<Use> pageFrameUseTime;
  for( int i = 0 ; i < pageframenum ; i++ ){
    Use temp_use;
    temp_use.counter = 0 ;
    temp_use.num = -1;
    pageFrameUseTime.push_back(temp_use);
  }
  
  for( int i = 0 ; i < Inputdata.size() ; i++ ){

    temp.num = Inputdata[i].num;
    int minpage = min( pageFrameUseTime ) ; 
    int j = 0 ;
    temp.pagefault = true;
    
    for( j ; j < pageframenum ; j++ ){
      if( pageFrameUseTime[j].num == temp.num ){
        temp.pagefault = false;
        minpage = j;
      }
    }
   
    if( temp.pagefault == true ){
      pageFrameUseTime[minpage].num = temp.num ;
      
      int tempnum = pageFrameUseTime[minpage].num;
      int tempcount = pageFrameUseTime[minpage].counter;
      
      for( int i = minpage ; i < pageFrameUseTime.size()-1 ; i++ ){
        pageFrameUseTime[i] = pageFrameUseTime[i+1] ;
      }
      
      pageFrameUseTime[pageFrameUseTime.size()-1].num = tempnum;
      pageFrameUseTime[pageFrameUseTime.size()-1].counter = tempcount;
      
    }
    else{
      pageFrameUseTime[minpage].counter++; 
      int tempnum = pageFrameUseTime[minpage].num;
      int tempcount = pageFrameUseTime[minpage].counter;
   
    }
    
    vector<int> temp_use;
    for( int i = 0 ; i < pageFrameUseTime.size() ; i++ ){
      temp_use.push_back(pageFrameUseTime[i].num) ;
    }
 
    temp.pageframeuse = temp_use;
    
    /*for( int i = 0 ; i < pageFrameUseTime.size() ; i++ )
      cout << pageFrameUseTime[i].num << " " <<pageFrameUseTime[i].counter << " " ;
    
    cout << endl;*/
      
    result.push_back(temp);
    temp_use.clear();
  }
}

void MFU(){
  Result temp;
  vector<Use> pageFrameUseTime;
  for( int i = 0 ; i < pageframenum ; i++ ){
    Use temp_use;
    temp_use.counter = 0 ;
    temp_use.num = -1;
    pageFrameUseTime.push_back(temp_use);
  }
  
  for( int i = 0 ; i < Inputdata.size() ; i++ ){

    temp.num = Inputdata[i].num;
    int minpage = max( pageFrameUseTime ) ;
    int j = 0 ;
    temp.pagefault = true;
    
    for( j ; j < pageframenum ; j++ ){
      if( pageFrameUseTime[j].num == temp.num ){
        temp.pagefault = false;
        minpage = j;
      }
    }
      
    if( temp.pagefault == true ){
      pageFrameUseTime[minpage].num = temp.num ;
      pageFrameUseTime[minpage].counter = 0;
      
      int tempnum = pageFrameUseTime[minpage].num;
      int tempcount = pageFrameUseTime[minpage].counter;
      
      for( int i = minpage ; i < pageFrameUseTime.size()-1 ; i++ ){
        pageFrameUseTime[i] = pageFrameUseTime[i+1] ;
      }

      pageFrameUseTime[pageFrameUseTime.size()-1].num = tempnum;
      pageFrameUseTime[pageFrameUseTime.size()-1].counter = tempcount;
    }
    else{
      pageFrameUseTime[minpage].counter++;  
      int tempnum = pageFrameUseTime[minpage].num;
      int tempcount = pageFrameUseTime[minpage].counter;
      
      for( int i = minpage ; i < pageFrameUseTime.size()-1 ; i++ ){
        pageFrameUseTime[i] = pageFrameUseTime[i+1] ;
      }
      
      pageFrameUseTime[pageFrameUseTime.size()-1].num = tempnum;
      pageFrameUseTime[pageFrameUseTime.size()-1].counter = tempcount;
    }
  
    vector<int> temp_use;
    for( int i = 0 ; i < pageFrameUseTime.size() ; i++ ){
      temp_use.push_back(pageFrameUseTime[i].num) ;
    }
  
    temp.pageframeuse = temp_use;
    /*for( int i = 0 ; i < pageFrameUseTime.size() ; i++ )
      cout << pageFrameUseTime[i].num << " " <<pageFrameUseTime[i].counter << " " ;
    
    cout << endl;*/ 
    result.push_back(temp);
    temp_use.clear();
  }
}


bool allone( vector<Use> pageFrameUseTime ){
  for( int i = 0 ; i < pageFrameUseTime.size() ; i++ )
    if( pageFrameUseTime[i].counter == 0 )
      return false;
      
  return true;
}


bool notfull( vector<Additional> addi , int & index ){
  for( int i = 0 ; i < addi.size() ; i++ ){
    if( addi[i].num == -1 ){
      index = i ;
      return true;
    }
  }
  return false;
}

int minShifter( vector<Additional> addi ){
  int min = 0 ;
  vector<Additional> temp = addi;
  
  for( int i = 0 ; i < temp.size() ; i++ ){
    for( int j = i+1 ; j < temp.size() ; j++ ){
      if( temp[j].shifter < temp[i].shifter ){
        swap( temp[j], temp[i] ) ;
      }
    }
  }
  
  for( int i = 0 ; i < addi.size() ; i++ )
    if( addi[i].num == temp[0].num )
      return i;
      
  return -123456 ;
  
}

int findpage( vector<Additional> addi, int num ) {
  for( int i = 0 ; i < addi.size() ; i++ ){
    if( addi[i].num == num )
      return i;
  }
  
  return -123456;
}

void ARB(){
  Result temp;
  vector<Additional> addi;
  for( int i = 0 ; i < pageframenum ; i++ ){
    Additional temp;
    temp.bit = 0 ;
    temp.num = -1;
    temp.shifter = 0;
    addi.push_back(temp);
  }
  
  
  for( int i = 0 ; i < Inputdata.size() ; i++ ){   
    int j = 0 ;
    
    temp.pagefault = true;
    addi[j].bit = 0 ;
    for( j ; j < pageframenum ; j++ ){
      if( addi[j].num == Inputdata[i].num ){
        temp.pagefault = false;
        addi[j].bit = 1 ;
      }
    }
    
    
    int index = 0 ;

    if( temp.pagefault == true ){ // 需要做頁置換 或者直接放   
      
      if( notfull(addi, index) ){ //若還沒滿 直接放 
        addi[index].num = Inputdata[i].num;
        addi[index].bit = 1;
        addi[index].shifter = 0;
      }
      else{  // 已經放滿 需要找shifter最小的來換 
        index = minShifter( addi );
        Additional current ;
        
        current.num = Inputdata[i].num;
        current.bit = 1;
        current.shifter = 0;
        
        addi.erase(addi.begin()+index);
        
        /*addi[index].num = Inputdata[i].num;
        addi[index].bit = 1;
        addi[index].shifter = 0;*/
        
        addi.push_back(current);  
      }
    }
    else{  
      index = findpage( addi, Inputdata[i].num ) ;
      addi[index].bit = 1 ;
    }
    vector<int> temp_int;
    for( int i = 0 ; i < addi.size() ; i++ ){
      addi[i].shifter = addi[i].shifter / 2;
      if( addi[i].bit == 1 ){
        addi[i].shifter = addi[i].shifter + pow( 2 , 16 ) ;
      }
      addi[i].bit = 0 ;
      temp_int.push_back(addi[i].num);
    }

    /*for( int i = 0 ; i < addi.size() ; i++ )
        cout << "num: " << addi[i].num << "  bit: " << addi[i].bit << "  shift: " << addi[i].shifter << endl;
       
    cout << endl; 
    */
        
    temp.num = Inputdata[i].num;
    temp.pageframeuse = temp_int;     
    result.push_back(temp);
    
    temp_int.clear();
  }
}

void SCP(){
  Result temp;
  vector<Use> pageFrameUseTime;
  for( int i = 0 ; i < pageframenum ; i++ ){
    Use temp_use;
    temp_use.counter = 0 ;
    temp_use.num = -1;
    temp_use.time = -1;
    pageFrameUseTime.push_back(temp_use);
  }
  
  int clock = 0 ;
  for( int i = 0 ; i < Inputdata.size() ; i++,clock++ ){
    Use temp_use;
    temp_use.num = Inputdata[i].num;
    temp.num = Inputdata[i].num;
    temp_use.counter = 1;
    temp_use.time = clock;
    
    int j = 0 ;
    
    temp.pagefault = true;
    
    for( j ; j < pageframenum ; j++ ){
      if( pageFrameUseTime[j].num == temp_use.num ){
        temp.pagefault = false;
      }
    }

    if( temp.pagefault == true ){
      
      int min;
      if( allone(pageFrameUseTime) ){
        min = min_scp( pageFrameUseTime );

        for( int j = 0 ; j < pageFrameUseTime.size() ; j++,clock++ ){
          if( j != min ){
            pageFrameUseTime[j].counter = 0 ;
            pageFrameUseTime[j].time = clock;
          }
        }
          
        pageFrameUseTime[min].num = temp_use.num;
        pageFrameUseTime[min].counter = 1; 
        pageFrameUseTime[min].time = clock;
        clock++;
        
      }
      else{
        min = min_scp2( pageFrameUseTime );
        
        for( int j = 0 ; j < min ; j++,clock++ ){
            pageFrameUseTime[j].counter = 0 ;
            pageFrameUseTime[j].time = clock;
        }
        
        pageFrameUseTime[min].num = temp_use.num;
        pageFrameUseTime[min].counter = 1;  
        pageFrameUseTime[min].time = clock; 
        clock++;
      }

    }
    else{
      
      for( int q = 0 ; q < pageFrameUseTime.size() ; q++ ){
        if( pageFrameUseTime[q].num == temp_use.num ){
          pageFrameUseTime[q].counter = 1; 
        }
      }
   
    }

    vector<int> temp_int;
    
    for( int q = 0 ; q < pageFrameUseTime.size() ; q ++ )
      for( int x = q+1 ; x < pageFrameUseTime.size() ; x ++ )
        if( pageFrameUseTime[x].time < pageFrameUseTime[q].time )
          swap( pageFrameUseTime[x],  pageFrameUseTime[q] ) ;
    
    for( int i = 0 ; i < pageFrameUseTime.size() ; i++ ){
      temp_int.push_back(pageFrameUseTime[i].num) ;
    }

    /*for( int i = 0 ; i < pageFrameUseTime.size() ; i++ )
        cout << pageFrameUseTime[i].num << " " <<pageFrameUseTime[i].counter << " " << pageFrameUseTime[i].time << endl;   
    cout << endl; 
    */
    
    temp.pageframeuse = temp_int;     
    result.push_back(temp);
    temp_int.clear();
  }
}


int main() {
  int command = 0 ;
  while( command == 0 ){
    command = Input();
    Inputdata.erase(Inputdata.begin());

    FIFO();
    int numofpagefault = 0 ;
    fprintf( fout, "--------------FIFO-----------------------\n" ) ;
    cout << "--------------FIFO-----------------------" << endl;
    for( int i = 0 ; i < result.size() ; i++ ){
      cout << result[i].num << "\t";
      fprintf( fout, "%d\t",result[i].num );
      for( int j = 0 ; j < result[i].pageframeuse.size() ; j++ ){
        cout << result[i].pageframeuse[j];
        fprintf( fout, "%d",result[i].pageframeuse[j] );
      }
      cout << "\t" ;
      fprintf( fout, "\t" );
      if( result[i].pagefault == true ){
        cout << "F" ;
        numofpagefault++;
        fprintf( fout, "F" );
      }
      cout << endl;
      fprintf( fout, "\n" );
    }
    cout << "Page Fault = " << numofpagefault << "  Page Replaces = " << numofpagefault - pageframenum << "  Page Frames = " << pageframenum << endl << endl; 
    fprintf( fout, "Page Fault = %d  Page Replaces = %d  Page Frames = %d\n\n",numofpagefault,numofpagefault - pageframenum,pageframenum );  
    numofpagefault = 0 ;
    result.clear();
    pageframe.clear();
    
    
    
    LRU();
    fprintf( fout, "--------------LRU-----------------------\n" ) ;
    cout << "--------------LRU-----------------------" << endl;
    for( int i = 0 ; i < result.size() ; i++ ){
      cout << result[i].num << "\t";
      fprintf( fout, "%d\t",result[i].num );
      for( int j = 0 ; j < result[i].pageframeuse.size() ; j++ ){
        cout << result[i].pageframeuse[j];
        fprintf( fout, "%d",result[i].pageframeuse[j] );
      }
      
      cout << "\t" ;
      fprintf( fout, "\t" );
      if( result[i].pagefault == true ){
        cout << "F" ;
        numofpagefault++;
        fprintf( fout, "F" );
      }
      
      cout << endl;
      fprintf( fout, "\n" );
    }
    
    cout << "Page Fault = " << numofpagefault << "  Page Replaces = " << numofpagefault - pageframenum << "  Page Frames = " << pageframenum << endl << endl; 
    fprintf( fout, "Page Fault = %d  Page Replaces = %d  Page Frames = %d\n\n",numofpagefault,numofpagefault - pageframenum,pageframenum );
    

    numofpagefault = 0 ;
    result.clear();
    pageframe.clear();
    
    ARB();
    fprintf( fout, "--------------Additional Reference Bits-------------------\n" ) ;
    cout << "--------------Additional Reference Bits-------------------" << endl;
    for( int i = 0 ; i < result.size() ; i++ ){
      cout << result[i].num << "\t";
      fprintf( fout, "%d\t",result[i].num );
      for( int j = result[i].pageframeuse.size()-1 ; j >= 0 ; j-- ){
        if( result[i].pageframeuse[j] != -1 ) {
          cout << result[i].pageframeuse[j];
          fprintf( fout, "%d",result[i].pageframeuse[j] );
        }
      }
        
      cout << "\t" ;
      fprintf( fout, "\t" );
      if( result[i].pagefault == true ){
        cout << "F" ;
        numofpagefault++;
        fprintf( fout, "F" );
      }
      cout << endl;
      fprintf( fout, "\n" );
    }
    cout << "Page Fault = " << numofpagefault << "  Page Replaces = " << numofpagefault - pageframenum << "  Page Frames = " << pageframenum << endl << endl; 
    fprintf( fout, "Page Fault = %d  Page Replaces = %d  Page Frames = %d\n\n",numofpagefault,numofpagefault - pageframenum,pageframenum );
    
    
    numofpagefault = 0 ;
    result.clear();
    pageframe.clear();
    
    SCP();
    fprintf( fout, "--------------Second chance Page-------------------------\n" ) ;
    cout << "--------------Second chance Page-------------------------" << endl;
    for( int i = 0 ; i < result.size() ; i++ ){
      cout << result[i].num << "\t";
      fprintf( fout, "%d\t",result[i].num );
      for( int j = result[i].pageframeuse.size()-1 ; j >= 0 ; j-- ){
        if( result[i].pageframeuse[j] != -1 ) {
          cout << result[i].pageframeuse[j];
          fprintf( fout, "%d",result[i].pageframeuse[j] );
        }
      }
        
      cout << "\t" ;
      fprintf( fout, "\t" );
      if( result[i].pagefault == true ){
        cout << "F" ;
        numofpagefault++;
        fprintf( fout, "F" );
      }
      cout << endl;
      fprintf( fout, "\n" );
    }
    cout << "Page Fault = " << numofpagefault << "  Page Replaces = " << numofpagefault - pageframenum << "  Page Frames = " << pageframenum << endl << endl; 
    fprintf( fout, "Page Fault = %d  Page Replaces = %d  Page Frames = %d\n\n",numofpagefault,numofpagefault - pageframenum,pageframenum );
  

    numofpagefault = 0 ;
    result.clear();
    pageframe.clear();

    LFU();
    fprintf( fout, "--------------Least Frequently Used Page Replacement --------------------\n" ) ;
    cout << "--------------Least Frequently Used Page Replacement --------------------" << endl;
    for( int i = 0 ; i < result.size() ; i++ ){
      cout << result[i].num << "\t";
      fprintf( fout, "%d\t",result[i].num );
      for( int j = result[i].pageframeuse.size()-1 ; j >= 0 ; j-- ){
        if( result[i].pageframeuse[j] != -1 ) {
          cout << result[i].pageframeuse[j];
          fprintf( fout, "%d",result[i].pageframeuse[j] );
        }
      }

      cout << "\t" ;
      fprintf( fout, "\t" );
      if( result[i].pagefault == true ){
        cout << "F" ;
        numofpagefault++;
        fprintf( fout, "F" );
      }
      
      cout << endl;
      fprintf( fout, "\n" );
    }
    
    cout << "Page Fault = " << numofpagefault << "  Page Replaces = " << numofpagefault - pageframenum << "  Page Frames = " << pageframenum << endl << endl; 
    fprintf( fout, "Page Fault = %d  Page Replaces = %d  Page Frames = %d\n\n",numofpagefault,numofpagefault - pageframenum,pageframenum );
    
    
    numofpagefault = 0 ;
    result.clear();
    pageframe.clear();
    
    MFU();
    fprintf( fout, "--------------Most Frequently Used Page Replacement ------------------------\n" ) ;
    cout << "--------------Most Frequently Used Page Replacement ------------------------" << endl;
    for( int i = 0 ; i < result.size() ; i++ ){
      cout << result[i].num << "\t";
      fprintf( fout, "%d\t",result[i].num );
      for( int j = result[i].pageframeuse.size()-1 ; j >= 0 ; j-- ){
        if( result[i].pageframeuse[j] != -1 ) {
          cout << result[i].pageframeuse[j];
          fprintf( fout, "%d",result[i].pageframeuse[j] );
        }
      }

      cout << "\t" ;
      fprintf( fout, "\t" );
      if( result[i].pagefault == true ){
        cout << "F" ;
        numofpagefault++;
        fprintf( fout, "F" );
      }
      
      cout << endl;
      fprintf( fout, "\n" );
    }
    cout << "Page Fault = " << numofpagefault << "  Page Replaces = " << numofpagefault - pageframenum << "  Page Frames = " << pageframenum << endl << endl; 
    fprintf( fout, "Page Fault = %d  Page Replaces = %d  Page Frames = %d\n\n",numofpagefault,numofpagefault - pageframenum,pageframenum );
    

    Inputdata.clear();
    result.clear();
    pageframe.clear();

    fclose(fout);
    fclose(fp);
  }
}

