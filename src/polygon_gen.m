x = rand(1,100)*5;
y = rand(1,100)*5;

P = transpose([x; y]);

DT = delaunay(P);

DTSize = size(DT);

polygons =  polyshape.empty(DTSize(1), 0);

for i = 1:DTSize(1)
    trianglePoints = transpose(P(DT(i, :), :));
    newPoly = polyshape(trianglePoints(1, :), trianglePoints(2, :));
    polygons(i) = newPoly;
end

plot(polygons)
