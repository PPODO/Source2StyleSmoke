#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SmokeVoxel.generated.h"

UCLASS()
class SOURCE2STYLESMOKE_API ASmokeVoxel : public AActor {
	GENERATED_BODY()
public:
	ASmokeVoxel();

private:
	UPROPERTY(VisibleDefaultsOnly)
	class UStaticMeshComponent* m_pSmokeVoxelMeshComponent;

};