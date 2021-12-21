

classdef meshConverter
% An Simple object that takes a Triangulation and converts it to a
% list of Polyshapes.
properties
    Data;
    Polygons;
end
methods
    function obj = meshConverter(Tr)
        %  Create object
        if nargin == 1
            if isa(Tr, "triangulation")
                obj.Data = Tr;
                dataSize = size(obj.Data.ConnectivityList);
                obj.Polygons = polyshape;
                for i=1:dataSize(1)
                   obj.Polygons = [obj.Polygons  polyshape];
                end
            else
                error("the parameter given is not a triangulation")
            end
        else
            error("you need only 1 parameter")
        end
    end
    % convertToPolyshape method implemented
    % as a MEX function
    obj = convertToPolyshape(obj)
end
end