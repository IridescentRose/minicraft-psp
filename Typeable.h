#pragma once

enum ClassType
{
	UNDEFINED,
	MOB,
	PLAYER,
	TOOL_ITEM,
	RESOURCE_ITEM,
	POWERGLOVE_ITEM,
	FURNITURE_ITEM,
	FURNITURE,
	AIR_WIZARD
};

//this class is intended to replace java instainceOf function using predefined enums
class Typeable {
public:
    virtual ~Typeable() {}

    bool instanceOf(ClassType t) const {
        return (this->classType() == t);
    }

    virtual ClassType classType() const {
        return UNDEFINED;
    }
};
