from numpy import *
from glob import glob
from re import findall
from numpy import where
from math import factorial


#wazny komentaz: up jest czescia zmiennych dotyczacych zwolnien, down zmiennych dotyczacych przyspieszen
def shuffled_runs(i, n):
    return (n*(i**2+3*i+1)/factorial(i+3)-(1**3+3*i**2-i-4)/(factorial(i+3)))
def read_data(reafile, columnRR, columnANNOT):
    reafile_current=open(reafile, 'r')
    first_line=reafile_current.readline()
    signal=[] # this variable contains the signal for spectral analysis
    annotation=[]
    #here the reading of the file starts
    for line in reafile_current:
        line_content=findall(r'\b[0-9\.]+', line)
        signal.append(float(line_content[columnRR-1]))
        if columnRR!=columnANNOT:
            annotation.append(int(float(line_content[columnANNOT-1])))
    signal=array(signal)
    if columnRR==columnANNOT:
        annotation=0*signal
    annotation=array(annotation)
    return signal ,annotation

print("""
      *************************************
      *    This is rea2runs ver. 0.05     *
      *        09.17.2010, 10:39          *
      *************************************
      
please select the type of filter you want to use
1 annotation filter
2 square filter
3 quotient filter
you can combine filters
""")
filtr=input('answer by giving the number corresponding to your filter e.g. 1 2 3: \n')
filtr=findall(r'\b[0-9]+', filtr)
filtr=array(map(int,filtr))

#teraz definiuje stan wybranych filtrow
if len(where(filtr==1)[0])==1:
    filtr_annot=True
    filtr_no_annot=False
else:
    filtr_annot=False
    filtr_no_annot=True
if len(where(filtr==2)[0])==1:
    filtr_square=True
else:
    filtr_square=False
if len(where(filtr==3)[0])==1:
    filtr_quot=True
else:
    filtr_quot=False

#teraz uzytkownik wybierze parametry filtrow

if filtr_annot==True:
    print("\n*** The annotation filter ***")
    print("""The rea files contain the following annotations:
    0 -- sinus beat
    1 -- ventricular beat
    2 -- supraventricular beat
    3 -- artifact
    Enter a list of the beats you want TO LEAVE in the analysis
    (sinus beats will be left automatically, so you do not need to enter 0)""")
    annot_leave=input()
    annot_leave=findall(r'\b[0-9]+', annot_leave)
    annot_leave=array(map(int,annot_leave))

if filtr_square==True:
    print("\n*** The square filter ***")
    up_bound=float(input('Enter the upper bound for the filter in ms (e.g. 2000): '))
    down_bound=float(input('Enter the lower bound for the filter in ms (e.g. 300): '))


if filtr_quot==True:
    print("\n*** The quotient filter ***")
    percentage=float(input('Enter the percentage which disqualifies a beat \n(e.g. 30 means that a beat will be disqualified,\ni.e. will break any run, if it is greater or smaller\nthan the preceding beat by more than 30%: '))

results=open('wyniki.csv','w')
first_line_up='1Dec\tNBeatN1Dec\tNSerieN1Dec\t2Dec\t3Dec\t4Dec\t5Dec\t6Dec\t7Dec\t8Dec\t9Dec\t10Dec\t11Dec\t12Dec\t13Dec\t14Dec\t15Dec\t16Dec\t17Dec\t18Dec\t19Dec\t20Dec\t21Dec\t22Dec\t23Dec\t24Dec\t25Dec\t26Dec\t27Dec\t28Dec\t29Dec\t30Dec\t'
first_line_down='1Acc\tNBeatN1Acc\tNSerieN1Acc\t2Acc\t3Acc\t4Acc\t5Acc\t6Acc\t7Acc\t8Acc\t9Acc\t10Acc\t11Acc\t12Acc\t13Acc\t14Acc\t15Acc\t16Acc\t17Acc\t18Acc\t19Acc\t20Acc\t21Acc\t22Acc\t23Acc\t24Acc\t25Acc\t26Acc\t27Acc\t28Acc\t29Acc\t30Acc\t'
first_line_theor='1Theor\t2Theor\t3Theor\t4Theor\t5Theor\t6Theor\t7Theor\t8Theor\t9Theor\t10Theor\t11Theor\t12Theor\t13Theor\t14Theor\t15Theor\t16Theor\t17Theor\t18Theor\t19Theor\t20Theor\t21Theor\t22Theor\t23Theor\t24Theor\t25Theor\t26Theor\t27Theor\t28Theor\t29Theor\t30Theor\t'
first_line='plik\t'+first_line_up+first_line_down+first_line_theor+'Nall\t'+'Nsinus\t'+'NDec\t'+'NAcc\t'+'Non\t'+'entDec\t'+'entAcc\t'+'entDecS\t'+'entAccS\t'+'ent_theor\t'+'ent_theorS\t'+'longest_Dec\t'+'longest_Acc\n'
results.write(first_line)

file_list=sort(glob('*RR.csv'))
numer_pliku=1
for plik in file_list:
    print(numer_pliku, "processing file", plik)
    numer_pliku=numer_pliku+1
    RR, anotacje=read_data(plik,1,2)

    #tutaj wprowadze filtry
    if filtr_no_annot:
        anotacje=anotacje*0

    if filtr_annot:
        for annot in annot_leave:
            indeksy_annot_leave=where(anotacje==annot)[0]
            anotacje[indeksy_annot_leave]=0
    if filtr_square:
       indeksy_up=where(RR>up_bound)[0]
       indeksy_down=where(RR<down_bound)[0]
       indeksy_square_up_down=r_[indeksy_down, indeksy_up]
       anotacje[indeksy_square_up_down]=5
    if filtr_quot:
        roznice=abs(diff(RR))
        reference=RR[1:]
        quota=roznice/reference
        indeksy_quota=where(quota*100>percentage)[0]+1
        anotacje[indeksy_quota]=6
    znaki=diff(RR)
    znaki=sign(znaki)
    akumulator_down=arange(0,40)*0
    akumulator_up=arange(0,40)*0
    #tu zaczynam filtrowac dane, tzn. filtry przerywaja serie
    indeksy_do_wyrzucenia1=array(where(anotacje!=0))[0]
    indeksy_do_wyrzucenia2=indeksy_do_wyrzucenia1-1
    #teraz zalatwie sie z ewentualnym wyrzutkiem na koncu
    if len(indeksy_do_wyrzucenia1)>=1 and max(indeksy_do_wyrzucenia1)>=len(znaki):
        bad_index=where(indeksy_do_wyrzucenia1==max(indeksy_do_wyrzucenia1))[0]
        indeksy_do_wyrzucenia1[bad_index]=len(znaki)-1

    indeksy_do_wyrzucenia=r_[indeksy_do_wyrzucenia1, indeksy_do_wyrzucenia2]
    znaki[indeksy_do_wyrzucenia]=16
    #temp_indeksy=where(anotacje!=0)[0]

    
    index_up=0
    index_down=0
    flaga=znaki[0] #pamieta popzredni znak
    
    for znak in znaki[1:]:
        if flaga==1 and znak==1:
            index_up+=1
        if flaga==-1 and znak==-1:
            index_down+=1
        if flaga==1 and znak!=1:
            akumulator_up[index_up]+=1
            index_up=0
        if flaga==-1 and znak!=-1:
            akumulator_down[index_down]+=1
            index_down=0
        flaga=znak

    if flaga==1 and znak==1:
        akumulator_up[index_up]+=1
    
    if flaga==-1 and znak==-1:
        akumulator_down[index_down]+=1

    NBeatN1up=sum(akumulator_up[1:]*(arange(1,40)+1))
    NSerieN1up=sum(akumulator_up[1:])
    NBeatN1down=sum(akumulator_down[1:]*(arange(1,40)+1))
    NSerieN1down=sum(akumulator_down[1:])
    
    output_string_up="%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d"%\
    (akumulator_up[0], NBeatN1up, NSerieN1up, akumulator_up[1], akumulator_up[2], akumulator_up[3], akumulator_up[4],\
     akumulator_up[5], akumulator_up[6], akumulator_up[7], akumulator_up[8], akumulator_up[9], akumulator_up[10], akumulator_up[11], akumulator_up[12], akumulator_up[13], akumulator_up[14], akumulator_up[15], akumulator_up[16], akumulator_up[17], akumulator_up[18], akumulator_up[19], akumulator_up[20], akumulator_up[21], akumulator_up[22], akumulator_up[23], akumulator_up[24],akumulator_up[25], akumulator_up[26], akumulator_up[27], akumulator_up[28], akumulator_up[29])

    output_string_down="%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d"%\
    (akumulator_down[0], NBeatN1down, NSerieN1down, akumulator_down[1], akumulator_down[2], akumulator_down[3], akumulator_down[4],\
     akumulator_down[5], akumulator_down[6], akumulator_down[7],akumulator_down[8], akumulator_down[9], akumulator_down[10], akumulator_down[11], akumulator_down[12], akumulator_down[13], akumulator_down[14], akumulator_down[15], akumulator_down[16], akumulator_down[17], akumulator_down[18], akumulator_down[19], akumulator_down[20], akumulator_down[21], akumulator_down[22], akumulator_down[23], akumulator_down[24],akumulator_down[25], akumulator_down[26], akumulator_down[27], akumulator_down[28], akumulator_down[29])
    Nsinus=len(where(znaki!=16)[0])
    output_string_theor="%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f"%(shuffled_runs(1+0, Nsinus), shuffled_runs(1+1,Nsinus), shuffled_runs(1+2,Nsinus), shuffled_runs(1+3,Nsinus), shuffled_runs(1+4,Nsinus),shuffled_runs(1+5,Nsinus), shuffled_runs(1+6,Nsinus), shuffled_runs(1+7,Nsinus),shuffled_runs(1+8,Nsinus), shuffled_runs(1+9,Nsinus), shuffled_runs(1+10,Nsinus), shuffled_runs(1+11,Nsinus), shuffled_runs(1+12,Nsinus), shuffled_runs(1+13,Nsinus), shuffled_runs(1+14,Nsinus), shuffled_runs(1+15,Nsinus), shuffled_runs(1+16,Nsinus), shuffled_runs(1+17,Nsinus), shuffled_runs(1+18,Nsinus), shuffled_runs(1+19,Nsinus), shuffled_runs(1+20,Nsinus), shuffled_runs(1+21,Nsinus), shuffled_runs(1+22,Nsinus), shuffled_runs(1+23,Nsinus), shuffled_runs(1+24,Nsinus),shuffled_runs(1+25,Nsinus), shuffled_runs(1+26,Nsinus), shuffled_runs(1+27,Nsinus), shuffled_runs(1+28,Nsinus), shuffled_runs(1+29,Nsinus))
    zera="%d"%len(where(znaki==0)[0])
    zatoki="%d"%len(where(znaki!=16)[0])
    Nup="%d"%(akumulator_up[0]+NBeatN1up)
    Ndown="%d"%(akumulator_down[0]+NBeatN1down)
    control_len="%d"%len(znaki)
    control_sum="%d"%((sum(akumulator_down*(arange(0,40)+1))+sum(akumulator_up*(arange(0,40)+1)))+len(where(znaki==0)[0]))

    #tu zostanie policzona delta entropii
    entUp=0
    entDown=0
    entUpS=0
    entDownS=0
    Nserii=sum(akumulator_up)#+sum(akumulator_down)
    Nsinus_theor=0
    for ii in arange(0,30):
        #tutaj sprawdzic roznice
        Nsinus_theor+=akumulator_up[ii]*(ii+1)+akumulator_down[ii]*(ii+1)
    for ii in arange(0,30):
        if akumulator_up[ii]>0:
            entUp+=akumulator_up[ii]*(float(ii)+1.)/Nsinus*log(akumulator_up[ii]*(float(ii)+1.)/Nsinus)
            entUpS+=float(akumulator_up[ii])/Nserii*log(float(akumulator_up[ii])/Nserii)
        if akumulator_down[ii]>0:
            entDown+=akumulator_down[ii]*(float(ii)+1)/Nsinus*log(akumulator_down[ii]*(float(ii)+1)/Nsinus)
            entDownS+=float(akumulator_down[ii])/Nserii*log(float(akumulator_down[ii])/Nserii)

    ent_theor=0
    
    #tu obliczamy liczbe zatok "teoretycznych", tzn. wynikajacych tylko z tych pobudzen, ktore sa czescia runs, a nie naleza do serii niezmiennych
    Nsinus_theor=0
    for ii in arange(0,30):
        Nsinus_theor+=akumulator_up[ii]*(ii+1)+akumulator_down[ii]*(ii+1)
        #Nsinus_theor=Nsinus
    #tu obliczamy liczbe serii teoretycznych
    Nserii_theor=0
    for ii in arange(0,9):
        Nserii_theor+=shuffled_runs(1+ii,Nsinus_theor)
    for ii in arange(0,30):
        ent_theor+=float(round(shuffled_runs(1+ii,Nsinus_theor)))*(ii+1)/Nsinus_theor*log(float(shuffled_runs(1+ii,Nsinus_theor)*(ii+1)/Nsinus_theor))
    ent_theorS=0
    for ii in arange(0,9):
        ent_theorS+=float(round(shuffled_runs(1+ii,Nsinus_theor)))/Nserii_theor*log(shuffled_runs(1+ii,Nsinus_theor)/Nserii_theor)
    string_ent="%f\t%f\t%f\t%f\t%f\t%f\t"%(-entUp,-entDown,-entUpS,-entDownS, -ent_theor, -ent_theorS)
    #tu policzone zostana najdluzsze serie
    longest_Dec=40
    longest_Acc=40
    indeks=39
    while akumulator_up[indeks]==0 and indeks>=0:
        longest_Dec-=1
        indeks-=1
    indeks=39
    while akumulator_down[indeks]==0 and indeks>=0:
        longest_Acc-=1
        indeks-=1
    string_longest="%d\t%d"%(longest_Dec, longest_Acc)
    output_string=plik+'\t'+output_string_up+'\t'+output_string_down+'\t'+output_string_theor+'\t'+control_len+'\t'+zatoki+'\t'+Nup+'\t'+Ndown+'\t'+zera+'\t'+string_ent+string_longest+'\n'
    results.write(output_string)
results.close()
