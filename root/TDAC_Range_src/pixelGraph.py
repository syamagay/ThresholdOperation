import ROOT
from ROOT import TGraph, TCanvas, TLegend, TStyle, gStyle

gStyle.SetPadTopMargin(0.10)
gStyle.SetPadLeftMargin(0.13)
gStyle.SetPadRightMargin(0.13)
gStyle.SetPadBottomMargin(0.12)

gStyle.SetTitleXOffset(1.0)
gStyle.SetTitleYOffset(1.2)

font=42
tsize=0.05
gStyle.SetTextFont(font);

gStyle.SetTextSize(tsize);
gStyle.SetLabelFont(font,"x");
gStyle.SetTitleFont(font,"x");
gStyle.SetLabelFont(font,"y");
gStyle.SetTitleFont(font,"y");
gStyle.SetLabelFont(font,"z");
gStyle.SetTitleFont(font,"z");

gStyle.SetLabelSize(tsize,"x");
gStyle.SetTitleSize(tsize,"x");
gStyle.SetLabelSize(tsize,"y");
gStyle.SetTitleSize(tsize,"y");
gStyle.SetLabelSize(tsize,"z");
gStyle.SetTitleSize(tsize,"z");

gStyle.SetNdivisions(508);

gStyle.SetPadTickX(1);
gStyle.SetPadTickY(1);

legend_tsize=0.04
gStyle.SetLegendTextSize(legend_tsize);

class pixelGraph():
    def __init__(self, col, row , name='', title=''):
        self.col=col
        self.row=row
        self.x_list=[]
        self.y_list=[]
        self.y_min=0
        self.y_max=0
        self.x_min=0
        self.x_max=0
        self.y_offset=0
        self.bool_xrange=False
        self.bool_yrange=False
        
        self.bool_title=False
        self.bool_name=False
        if not name=='' :
            self.SetName(name)

        if not title=='' :
            self.SetTitle(title)

    def SetTitle(self, title):
        self.title=title
        self.bool_title=True
    def SetName(self, name):
        self.name=name
        self.bool_name=True
    def SetXRange(self,xmin,xmax):
        self.x_min=xmin
        self.x_max=xmax
        self.bool_xrange=True

    def SetYRange(self,ymin,ymax):
        self.y_min=ymin
        self.y_max=ymax
        self.bool_yrange=True
    def SetYOffset(self, val):
        self.y_offset=val
    def SetPoint(self,x,y):
        self.x_list.append(x)
        self.y_list.append(y)
    def CreateGraph(self):
        self.pgraph=TGraph(len(self.x_list))
        if self.bool_name :
            self.pgraph.SetName(self.name)

        iEntry=0
        for (x,y) in zip(self.x_list,self.y_list):
            self.pgraph.SetPoint(iEntry,x,y-self.y_offset)
            iEntry=iEntry+1
        if self.bool_title :
            self.pgraph.SetTitle(self.title)
        self.pgraph.GetYaxis().SetTitleOffset(1.3)

        if self.bool_xrange :
            self.pgraph.GetXaxis().SetLimits(self.x_min, self.x_max)
        if self.bool_yrange :
            self.pgraph.SetMinimum(self.y_min)
            self.pgraph.SetMaximum(self.y_max)


        return self.pgraph

class AnalysisBasic():
    def __init__(self,name):
        self.graph_list=[]
        self.name=name
        self.Value1List=[]
        self.Value2List=[[]]
        self.Value3List=[[]]

        self.col=0
        self.row=0

        self.SetLegendCoordinate()
        self.SetLegendStatus()
        self.SetTitle()
        try :
            self.ValueName1
        except :
            self.ValueName1='colrow'
        try :
            self.ValueName2
        except :
            self.ValueName2='TDAC'
        try :
            self.xAxis
        except :
            self.xAxis='x-axis'
        try :
            self.yAxis
        except :
            self.yAxis='y-axis'
    def SetTitle(self):
        self.graph_title=self.name
    def SetLegendCoordinate(self):
        self.legend_xl=0.68
        self.legend_xr=0.90
        self.legend_yb=0.2
        self.legend_yt=0.83
    def SetLegendStatus(self):
        self.legend_Col=1
        self.legend_Border=1
        
    def SetRange(self):
        pass
    def CreatePixelGraph(self, col, row , name, title):
        pg=pixelGraph(col,row,name,title)
        xmin, xmax, ymin, ymax=self.SetRange()
        pg.SetXRange(xmin,xmax)
        pg.SetYRange(ymin,ymax)
        return pg
    def CreateNameTitle(self, Value_1, current_Value_2):
        #pg_name=self.name+'_'+self.ValueName1+'_'+str(Value_1)+'_'+self.ValueName2+'_'+str(current_Value_2)
        pg_name=self.name+'_'+self.ValueName1+'_'+str(Value_1)
        #pg_title=self.name+'_'+self.ValueName1+'_'+str(Value_1)+'_'+self.ValueName2+'_'+str(current_Value_2)+';'+self.xAxis+';'+self.yAxis
        #pg_title=self.name+'_'+self.ValueName1+'_'+str(Value_1)+';'+self.xAxis+';'+self.yAxis
        pg_title=self.graph_title+';'+self.xAxis+';'+self.yAxis
        return pg_name, pg_title
        
    def SetPixelGraph(self, current_Value1, current_Value_2):
        exist_Value1=False
        i_Value1=0
        i=0
        for Value1 in self.Value1List :
            if current_Value1 == Value1:
                exist_Value1 = True
                break
            i=i+1
        i_Value1=i
        if not exist_Value1 :
            self.Value1List.append(current_Value1)
            self.graph_list.append([])
            self.Value2List.append([])
            self.Value3List.append([])

        exist_Value2=False
        i=0
        i_Value2=0
        for Value2 in self.Value2List[i_Value1] :
            if current_Value_2 == Value2 :
                exist_Value2 = True
                break
            i=i+1
        i_Value2=i
        if not exist_Value2 :
            pg_name, pg_title=self.CreateNameTitle(current_Value1, current_Value_2)
            pg=self.CreatePixelGraph(self.col,self.row, pg_name, pg_title)
            self.Value2List[i_Value1].append(current_Value_2)
            self.Value3List[i_Value1].append(current_Value_2)
            self.graph_list[i_Value1].append(pg)
        return i_Value1, i_Value2
            
    def SetEvent(self, col, row, target_threshold, Threshold, Noise, TDAC, DiffVth1):
        pass
    def SetValue3(self,i,j,val):
        self.Value3List[i][j]=int(val)
    def Draw(self):
        canvas=TCanvas(self.name+'canvas','')
        canvas.Print('picture/'+self.name+'.pdf[')
        j=0
        text_name=''
        textvalue=''
        if(self.ValueName1=='colrow'):
            text_name='(col, row)'
        else:
            text_name=self.ValueName1

        text2_name=''
        text2value=''
        if(self.ValueName2=='colrow'):
            #text2_name='#splitline{pixel coordinates}{(col,row)}'
            text2_name='pixel(column,row)'
        else:
            text2_name=self.ValueName2

        for graph_list_1 in self.graph_list:
            legend=TLegend(self.legend_xl,self.legend_yb,self.legend_xr,self.legend_yt,text2_name)
            legend.SetNColumns(self.legend_Col)
            legend.SetBorderSize(self.legend_Border)
            i=1
            for pixel_graph in graph_list_1:
                graph=pixel_graph.CreateGraph()
                graph.SetLineColor(i)
                graph.SetMarkerColor(i)
                graph.SetMarkerStyle(8)
                if(self.ValueName2=='colrow'):
                    text2value='('+str(int(self.Value3List[j][i-1]/1000))+', '+str(self.Value3List[j][i-1]%1000)+')'
                else:
                    text2value=str(self.Value3List[j][i-1])

                legend.AddEntry(graph,text2value,'lp')
                canvas.cd()

                if i == 1 :
                    #graph.Draw("APL")
                    graph.Draw("AP")
                else :
                    #graph.Draw("PL")
                    graph.Draw("P")
                i=i+1
            legend.Draw('same')
            if(self.ValueName1=='colrow'):
                textvalue='('+str(int(self.Value1List[j]/1000))+', '+str(self.Value1List[j]%1000)+')'
            else:
                textvalue=str(self.Value1List[j])
            text=ROOT.TText(0.18,0.83,text_name+'='+textvalue)
            text.SetTextFont(62)
            text.SetNDC(1)
            text.Draw()
            canvas.Print('picture/'+self.name+'.pdf')
            j=j+1
        canvas.Print('picture/'+self.name+'.pdf]')

class Threshold_vs_Vth1_forTDAC(AnalysisBasic):
    def __init__(self):
        self.ValueName1='colrow'
        self.ValueName2='VTH2=50, TDAC'
        self.xAxis='VTH1 [Register Value]'
        self.yAxis='Threshold [e]'
        super().__init__('Threshold_vs_Vth1_forTDAC')
    def SetRange(self):
        xmin=0
        xmax=600
        ymin=0
        ymax=4000
        return xmin,xmax,ymin,ymax
    def SetEvent(self, col, row, target_threshold, Threshold, Noise, TDAC, DiffVth1):
        self.col=col
        self.row=row
        colrow=col*1000+row
        i_colrow, i_TDAC=self.SetPixelGraph(colrow,TDAC)
        self.graph_list[i_colrow][i_TDAC].SetPoint(DiffVth1,Threshold)

class Threshold_vs_TDAC_forVth1(AnalysisBasic):
    def __init__(self):
        self.ValueName1='colrow'
        self.ValueName2='DiffVth1'
        self.xAxis='TDAC'
        self.yAxis='Threshold'
        super().__init__('Threshold_vs_TDAC_forVth1')
    def SetRange(self):
        xmin=-15
        xmax=25
        ymin=0
        ymax=4000
        return xmin,xmax,ymin,ymax
    def SetEvent(self, col, row, target_threshold, Threshold, Noise, TDAC, DiffVth1):
        self.col=col
        self.row=row
        colrow=col*1000+row
        i_colrow, i_Vth1=self.SetPixelGraph(colrow,DiffVth1)
        self.graph_list[i_colrow][i_Vth1].SetPoint(TDAC,Threshold)

class Threshold_vs_Vth1_forPixel(AnalysisBasic):
    def __init__(self):
        self.ValueName1='VTH2=50 TDAC'
        self.ValueName2='colrow'
        self.xAxis='VTH1 [Register Value]'
        self.yAxis='Threshold [e]'
        super().__init__('Threshold_vs_Vth1_forPixel')
    def SetRange(self):
        xmin=0
        xmax=450
        ymin=10
        ymax=3000
        return xmin,xmax,ymin,ymax

    def SetTitle(self):
        self.graph_title='Threshold_vs_VTH1'

    def SetLegendCoordinate(self):
        self.legend_xl=0.18
        self.legend_xr=0.55
        self.legend_yb=0.50
        self.legend_yt=0.82
    def SetLegendStatus(self):
        self.legend_Col=2
        self.legend_Border=0

    def SetEvent(self, col, row, target_threshold, Threshold, Noise, TDAC, DiffVth1):
        self.col=col
        self.row=row
        colrow=col*1000+row
        i_colrow, i_TDAC=self.SetPixelGraph(TDAC,colrow)
        self.graph_list[i_colrow][i_TDAC].SetPoint(DiffVth1,Threshold)


class DiffThreshold_vs_Vth1_forTDAC(AnalysisBasic):
    def __init__(self):
        self.ValueName1='colrow'
        self.ValueName2='TDAC'
        self.xAxis='DiffVth1'
        self.yAxis='Threshold-target threshold'
        super().__init__('DiffThreshold_vs_Vth1_forTDAC')
    def SetRange(self):
        xmin=0
        xmax=600
        ymin=-1500
        ymax=2500
        return xmin,xmax,ymin,ymax
    def SetEvent(self, col, row, target_threshold, Threshold, Noise, TDAC, DiffVth1):
        self.col=col
        self.row=row
        colrow=col*1000+row
        i_colrow, i_TDAC=self.SetPixelGraph(colrow,TDAC)
        self.graph_list[i_colrow][i_TDAC].SetPoint(DiffVth1,Threshold-target_threshold)

class DiffThreshold_vs_TDAC_forVth1(AnalysisBasic):
    def __init__(self):
        self.ValueName1='colrow'
        #self.ValueName2='DiffVth1'
        #self.ValueName2='target_threshold'
        self.ValueName2='base threshold [e]'
        self.xAxis='TDAC'
        self.yAxis='Threshold - base'
        super().__init__('DiffThreshold_vs_TDAC_forVth1')

    def SetTitle(self):
        self.graph_title='DiffThreshold_vs_TDAC'

    def SetLegendCoordinate(self):
        self.legend_xl=0.18
        self.legend_xr=0.50
        self.legend_yb=0.55
        self.legend_yt=0.80
    def SetLegendStatus(self):
        self.legend_Col=2
        self.legend_Border=0

    def SetRange(self):
        xmin=-15
        xmax=15
        ymin=-1500
        ymax=2000
        return xmin,xmax,ymin,ymax
    def SetEvent(self, col, row, target_threshold, Threshold, Noise, TDAC, DiffVth1):
        self.col=col
        self.row=row
        colrow=col*1000+row
        #i_colrow, i_Vth1=self.SetPixelGraph(colrow,DiffVth1)
        i_colrow, i_Vth1=self.SetPixelGraph(colrow,target_threshold)
        #print(str(i_Vth1)+" "+str(target_threshold))
        self.graph_list[i_colrow][i_Vth1].SetPoint(TDAC,Threshold)
        if col==267 and row==18 :
            print('(267,18) TDAC='+str(TDAC)+', Threshold='+str(Threshold))
        if TDAC == 0 :
            self.SetValue3(i_colrow, i_Vth1, Threshold)
            self.graph_list[i_colrow][i_Vth1].SetYOffset(Threshold)

class DiffThreshold_vs_TDAC_forPixel(AnalysisBasic):
    def __init__(self):
        self.ValueName1='DiffVth1'
        self.ValueName2='colrow'
        self.xAxis='TDAC'
        self.yAxis='Threshold-target threshold'
        super().__init__('DiffThreshold_vs_TDAC_forPixel')
    def SetRange(self):
        xmin=-15
        xmax=25
        ymin=-1500
        ymax=2500
        return xmin,xmax,ymin,ymax
    def SetEvent(self, col, row, target_threshold, Threshold, Noise, TDAC, DiffVth1):
        self.col=col
        self.row=row
        colrow=col*1000+row
        i_Vth1,i_colrow=self.SetPixelGraph(DiffVth1,colrow)
        self.graph_list[i_Vth1][i_colrow].SetPoint(TDAC,Threshold-target_threshold)


class DiffThreshold_vs_Vth1_forPixel(AnalysisBasic):
    def __init__(self):
        self.ValueName1='TDAC'
        self.ValueName2='colrow'
        self.xAxis='DiffVth1'
        self.yAxis='Threshold-target threshold'
        super().__init__('DiffThreshold_vs_Vth1_forPixel')
    def SetRange(self):
        xmin=0
        xmax=600
        ymin=-1500
        ymax=2500
        return xmin,xmax,ymin,ymax
    def SetEvent(self, col, row, target_threshold, Threshold, Noise, TDAC, DiffVth1):
        self.col=col
        self.row=row
        colrow=col*1000+row
        i_TDAC, i_colrow=self.SetPixelGraph(TDAC,colrow)
        self.graph_list[i_TDAC][i_colrow].SetPoint(DiffVth1,Threshold-target_threshold)




if __name__=='__main__' :
    AB_list=[]
    AB_list.append(DiffThreshold_vs_Vth1_forPixel())
    AB_list.append(DiffThreshold_vs_Vth1_forTDAC())
    AB_list.append(DiffThreshold_vs_TDAC_forVth1())
    
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


