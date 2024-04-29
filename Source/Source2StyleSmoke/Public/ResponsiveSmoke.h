#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResponsiveSmoke.generated.h"

UCLASS()
class SOURCE2STYLESMOKE_API AResponsiveSmoke : public AActor {
	GENERATED_BODY()

public:
	AResponsiveSmoke();

public:
	UFUNCTION(BlueprintCallable)
	void GenerateSmoke();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float fDeltaTime) override;

private:
	void CalculateVoxel(float fDeltaTime);

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), DisplayName = "Grid Resolution")
	int m_iGridResolution = 6;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), DisplayName = "Target Volume Diameter")
	float m_fTargetVolumeDiameter = 300.0;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), DisplayName = "Snap To Grid")
	bool m_bSnapToGrid = true;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), DisplayName = "Voxel Spread Rate")
	float m_fVoxelSpreadRate = 0.125f;

	bool m_bIsExpansion;

private:
	TArray<AActor*> m_actorToIgnore;

	TArray<FVector> m_vListOfCheckAxes;

	std::unique_ptr<TArray<FVector>> m_vNextPositionQueue;
	std::unique_ptr<TArray<FVector>> m_vCurrentCheckPositionQueue;
	TArray<FVector> m_vCheckedPositions;
	TArray<FVector> m_vAcceptedPositions;

	int m_iTotalEnergy;
	float m_fCellSize;
	float m_fElpasedDeltaTime;

};