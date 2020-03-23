from pixelGraph import *
import ROOT
import sys

def Loop(filename):
    f=ROOT.TFile(filename)
    #f=ROOT.TFile('1109_TDAC_Range/analysis.root')
    #f=ROOT.TFile('0107_OU076A_TDAC_Range/analysis.root')
    #f=ROOT.TFile('1109_additional_Range/analysis.root')
    tree=f.Get('tree')
    total_event=tree.GetEntries()

    AnalysisList=[]
    AnalysisList.append(Threshold_vs_Vth1_forPixel())
#    AnalysisList.append(DiffThreshold_vs_Vth1_forPixel())
#    AnalysisList.append(DiffThreshold_vs_Vth1_forTDAC())
#    AnalysisList.append(DiffThreshold_vs_TDAC_forVth1())
#    AnalysisList.append(DiffThreshold_vs_TDAC_forPixel())

    tree.Draw('>>elist','Column>263')
    elist=ROOT.gROOT.FindObject('elist')

    total_entry=elist.GetN()

    Threshold_TDAC15=[]
    Threshold_TDACm15=[]
    target_threshold_list=[]

    for i_event in range(0, total_entry):
        tree.GetEntry(elist.GetEntry(i_event))
        if tree.TDAC == 15 or tree.TDAC == -15 :
            i_target_threshold=0
            if not tree.DiffVth1 in target_threshold_list :
                i_target_threshold=len(target_threshold_list)
                target_threshold_list.append(tree.DiffVth1)
                Threshold_TDAC15.append([])
                Threshold_TDACm15.append([])
            else:
                i=0
                for tt in target_threshold_list :
                    if tt == tree.DiffVth1 :
                        i_target_threshold=i
                    i=i+1
            if tree.TDAC ==15 :
                #print('col = '+str(tree.Column)+'row = '+str(tree.Row)+'TDAC = '+str(tree.TDAC))
                Threshold_TDAC15[i_target_threshold].append(tree.Threshold)
            else:
                #print('col = '+str(tree.Column)+'row = '+str(tree.Row)+'TDAC = '+str(tree.TDAC))
                Threshold_TDACm15[i_target_threshold].append(tree.Threshold)

        # using Analysis Class
        #if ( tree.Column==267 and tree.Row > 50 and tree.Row < 70 ) or ( tree.Column==387 and tree.Row==65 ) or ( tree.Column==396 and tree.Row==175 ): 
        #if tree.Column==300 and tree.Row > 35 and tree.Row < 45 :
        if tree.Column==267 and tree.Row%5==3 and tree.Row < 45 :
            for Analysis in AnalysisList:
                Analysis.SetEvent(tree.Column, tree.Row, 0, tree.Threshold, tree.Noise, tree.TDAC,tree.DiffVth1)

    for Analysis in AnalysisList:
        Analysis.Draw()

    #Evaluate TDAC Range
    c=ROOT.TCanvas('TDAC_Range_c','')
    hist=ROOT.TH2F('TDACRange','TDAC Range;target_threshold;TDAC Rangee',10,200,2200,100,0,4000)

    i_tt=0
    for tt in target_threshold_list :
        for i in range(0, len(Threshold_TDAC15[i_tt])):
            if Threshold_TDACm15[i_tt][i] > 0 :
                TDAC_Range=Threshold_TDAC15[i_tt][i]-Threshold_TDACm15[i_tt][i]
                #TDAC_Range=Threshold_TDACm15[i_tt][i]
                hist.Fill(tt, TDAC_Range)
        i_tt=i_tt+1
    c.cd()
    c.SetLogz()
    hist.Draw('colz')
    c.Print('picture/TDAC_Range.pdf')

if __name__=='__mein__' :
    AB_list=[]
    AB_list.append(DiffThreshold_vs_Vth1_forPixel())
    AB_list.append(DiffThreshold_vs_Vth1_forTDAC())
    AB_list.append(DiffThreshold_vs_TDAC_forVth1())
    AB_list.append(DiffThreshold_vs_TDAC_forPixel())

    for AB in AB_list :
        AB.SetEvent(10,10,1000,1000,100,4,150)
        AB.SetEvent(10,10,900,900,100,4,120)
        AB.SetEvent(10,10,800,800,100,4,70)
        AB.SetEvent(10,10,700,700,100,4,0)

        AB.SetEvent(10,10,1000,1000,100,5,150)
        AB.SetEvent(10,10,900,900,100,5,100)
        AB.SetEvent(10,10,800,800,100,5,50)
        AB.SetEvent(10,10,700,700,100,5,0)

        AB.SetEvent(7,10,1000,1000,100,4,140)
        AB.SetEvent(7,10,900,900,100,4,120)
        AB.SetEvent(7,10,800,800,100,4,30)
        AB.SetEvent(7,10,700,700,100,4,20)

        AB.Draw()
if __name__=='__main__' :
    args=sys.argv
    target_file=''
    if len(args) == 1 :
        target_file='1109_TDAC_Range/analysis.root'
    else:
        target_file=args[1]
    Loop(target_file)
