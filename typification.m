
% Typification using the maximum area strategy

src=imread('superpixel11_SLIC.png');
srcbuildings=imread('Ponds.png');
writenanme='typification_SLIC.png';
file_path='Results/';

bw=im2bw(src);
[L,num] = bwlabel(bw,8);

bw2=im2bw(srcbuildings);
[L2,num2] = bwlabel(bw2,8);

newbuildings=[];

for i=1:num
    
    i
    num
    try
        maxbuildings=[];
        maxarea=0;
        centrex=0;
        centrey=0;
        
        
        outTemp=(L==i);
        
        if i==1
            %新的池塘申明为空白
            [shenx,sheny]=find(outTemp==1);
            newbuildings=outTemp;
            for shen=1:length(shenx)
                newbuildings(shenx(shen),sheny(shen))=0;
                
                
            end
            
            
        end
        %                 figure(1);
        
        %          imshow(srcbuildings); hold on
        supixelCentre = regionprops(outTemp,'Centroid');
        centrex=supixelCentre.Centroid(1,1);
        centrey=supixelCentre.Centroid(1,2);
        %           text(centrex,centrey,['(',num2str(centrex),',',num2str(centrey),',',num2str(i),')'],'color','r');
        
        %             plot(xy(:,1),xy(:,2),'LineWidth',1.0,'Color','green');
        %             plot(centrex,centrey,'s','color','k');
        %当前的超像素的x和y坐标
        [xSuper,ySuper]=find(outTemp==1);
        
        
        
        for j=1:num2
            outTemp2=(L2==j);
            %             imshow(outTemp2);
            
            %当前的小池塘,获取其x和y坐标
            [x,y]=find(outTemp2==1);
            tag=0;
            %当前池塘的面积
            Current = regionprops(outTemp2,'Area');
            CurrentAREA=Current.Area;
            OverlayAREA=0;
            
            
            %如果当前的超像素与池塘重叠，则标记该池塘所在的连通域
            for u=1:length(x)
                for v=1:length(xSuper)
                    if (x(u)==xSuper(v)) && (y(u)==ySuper(v))
                        %得到与超像素相交的一个池塘
                        tag=1;
                        OverlayAREA=OverlayAREA+1;
                        
                        %                         break;
                    end
                end
                
                %                 if tag==1
                %                     break;
                %                 end
                
            end
            
            
            
            RatioAREA=OverlayAREA/CurrentAREA;
            
            if RatioAREA>=0.5
                tag=1;
            else
                tag=0;
            end
            
            
            if tag==1  %该连通域与超像素相交
                temparea = regionprops(outTemp2,'Area');
                j
                if temparea.Area>=maxarea
                    maxarea=temparea.Area;
                    maxbuildings=outTemp2;
                    
                end
                
                
                %  imshow(outTemp2);
            end
            
            
        end
        
        maxCentre = regionprops(maxbuildings,'Centroid');
        maxcentrex=maxCentre.Centroid(1,1);
        maxcentrey=maxCentre.Centroid(1,2);
        %                     plot(maxcentrex,maxcentrey,'s','color','k');
        %   text(maxcentrex,maxcentrey,['(',num2str(maxcentrex),',',num2str(maxcentrey),',',num2str(j),')'],'color','b');
        
        %移位
        disx1= maxcentrex-centrex ;
        disy1= maxcentrey-centrey ;
        
        disx=round(maxcentrex-centrex);
        disy=round(maxcentrey-centrey);
        
        
        [originalx,originaly]=find(maxbuildings==1);
        %         imwrite(maxbuildings,strcat (file_path,'before.png'));
        
        for w=1:length(originalx)
            %             maxbuildings(originalx(w),originaly(w))=0;
            
            originalx(w)=originalx(w)-disy;
            originaly(w)=originaly(w)-disx;
            
        end
        
        for w=1:length(originalx)
            newbuildings(originalx(w),originaly(w))=1;
        end
        
        
        
    catch
        continue;
    end
   
end

imwrite(newbuildings,strcat (file_path,writenanme));

figure(2);
imshow(newbuildings);

