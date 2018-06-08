function myfunc(param)
{

var x=document.getElementById(param);
var y=x.id;
var plus="plus";
var minus="minus";
var plusc= plus.concat(y);
var minusc=minus.concat(y);
var newminus=document.getElementById(minusc);
var newplus=document.getElementById(plusc);

     if (x.style.display === 'none')
     {
        x.style.display = 'block';
        newminus.style.display='none';
        newplus.style.display='block';
        
     }
    else 
    {
        x.style.display = 'none'; 
        newminus.style.display='block';
        newplus.style.display='none';
        
    }
  
  
  }