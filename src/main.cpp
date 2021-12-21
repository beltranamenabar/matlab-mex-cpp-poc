#include "mex.hpp"
#include "mexAdapter.hpp"

class MexFunction : public matlab::mex::Function {

public:
    MexFunction() : matlabPtr(getEngine()) {}

    void operator()(matlab::mex::ArgumentList outputs, matlab::mex::ArgumentList inputs) {

        checkArguments(inputs);

        matlab::data::Array obj(inputs[0]);
        matlab::data::Array data(matlabPtr->getProperty(obj, u"Data"));

        stream << "Data dimensions" << data.getDimensions()[0] << " " << data.getDimensions()[1];
        this->displayOnMATLAB(stream);

        matlab::data::Array points = matlabPtr->getProperty(data, u"Points");
        matlab::data::Array triangles = matlabPtr->getProperty(data, u"ConnectivityList");

        stream << "points dimensions" << points.getDimensions()[0] << " " << points.getDimensions()[1];
        stream << "triangles dimensions" << triangles.getDimensions()[0] << " " << triangles.getDimensions()[1];
        this->displayOnMATLAB(stream);

        matlab::data::Array polygons = matlabPtr->getProperty(obj, u"Polygon");

        stream << "polygons dimensions" << polygons.getDimensions()[0] << " " << polygons.getDimensions()[1];
        this->displayOnMATLAB(stream);

        for (int i = 0; i < polygons.getDimensions()[0]; i++) {

            unsigned int p0 = triangles[i][0];
            unsigned int p1 = triangles[i][1];
            unsigned int p2 = triangles[i][2];

            auto polygon = factory.createArray(
                {3, 2},
                {double(points[p0][0]), double(points[p1][0]), double(points[p2][0]),
                 double(points[p0][1]), double(points[p1][1]), double(points[p2][1])});
            matlabPtr->setProperty(polygons, i, u"Vertices", polygon);
        }
        // Return modified object
        outputs[0] = obj;
    }

private:
    std::shared_ptr<matlab::engine::MATLABEngine> matlabPtr;
    std::ostringstream stream;
    matlab::data::ArrayFactory factory;

    /**
     * @brief Checks if the input arguments are valid.
     *
     * @param inputs The inputs to be checked
     */
    void checkArguments(matlab::mex::ArgumentList inputs) {

        // You only have to pass the object
        if (inputs.size() != 1) {
            matlabPtr->feval(u"error",
                             0,
                             std::vector<matlab::data::Array>({factory.createScalar("Only one input required")}));
        }

        // Check if the object is a meshConverter
        std::vector<matlab::data::Array> args{inputs[0], factory.createCharArray("meshConverter")};
        matlab::data::TypedArray<bool> result = matlabPtr->feval(u"isa", args);
        if (result[0] != true) {
            matlabPtr->feval(u"error", 0,
                             std::vector<matlab::data::Array>({factory.createScalar("Input must be meshConverter object")}));
        }
    }

    /**
     * @brief A simple utility function to display the string on MATLAB
     *
     * @param stream A stringstream object with the msg to be displayed
     */
    void displayOnMATLAB(std::ostringstream &stream) {
        // Pass stream content to MATLAB fprintf function
        matlabPtr->feval(u"fprintf", 0,
                         std::vector<matlab::data::Array>({factory.createScalar(stream.str())}));
        // Clear stream buffer
        stream.str("");
    }
};