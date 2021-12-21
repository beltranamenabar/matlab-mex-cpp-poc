x = rand(1,100)*5;
y = rand(1,100)*5;

P = transpose([x; y]);

DT = delaunay(P);

DTSize = size(DT);

TR = triangulation(DT, P);

MR = meshConverter(TR);
