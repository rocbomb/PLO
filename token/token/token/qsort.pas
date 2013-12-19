
var a: array [10] of integer;
index,num,temp:integer;
procedure qsort(l,h:integer);
var i,j,t,m:integer;
	procedure swap(var i, j:integer);
	var temp:integer;
	begin temp:=i;i:=j;j:=temp end;

begin
	i:=l;
	j:=h;
	m:=a[(i+j) / 2];
	while i<j do
	begin
		while a[i] < m do i:=i+1;
		while m < a[j] do j:=j-1  ;

		if i<=j then
		begin
		swap(a[i],a[j]);
		i:=i+1;
		j:=j-1;
		end;
	end;

	if j>l then qsort(l,j);
	if i<h then qsort(i,h);
end;

begin
write("please input num <10 ");
read(num);
for index:=1 to num do begin 
write("please input number> ");read(temp); a[index-1]:=temp end;

qsort(0, num-1);
for index:=0 to num-1 do write(a[index]);
end.

